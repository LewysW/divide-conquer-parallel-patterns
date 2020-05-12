#include "Worker.h"
#include "List.h"

//Define templates used by existing subclasses
template class Worker<int, int>;
template class Worker<List, List>;

//Stores the number of workers which are currently active (in the process of executing tasks)
//Is static so is shared between all Worker instances
template <typename ProblemType, typename ResultType>
std::set<Worker<ProblemType, ResultType>*> Worker<ProblemType, ResultType>::activeWorkers;

//Lock to ensure safe access to the activeWorkers data structure
template <typename ProblemType, typename ResultType>
std::mutex Worker<ProblemType, ResultType>::activeMutex;

/**
 * Solves a given problem using the parallel pattern of this object
 * @tparam ProblemType - type of problem to solve
 * @tparam ResultType - result of solved problem
 * @param p - to solve
 * @return result of problem
 */
template<typename ProblemType, typename ResultType>
ResultType Worker<ProblemType, ResultType>::solve(ProblemType p) {
    //If base condition
    if (this->threshold(p)) {
        //Return base solution
        return this->base(p);
    } else {
        //Otherwise split problem p into problems ps
        std::vector<ProblemType> ps;
        ps = this->divide(p);

        //For each problem
        for (int i = 0; i < ps.size(); i++) {
            //Create task using problem
            std::shared_ptr<Task<ProblemType, ResultType>> taskPtr =
                    std::make_shared<Task<ProblemType, ResultType>>(Task<ProblemType, ResultType>(ps[i]));
            //Add the task to the input queue
            putTaskInputQueue(taskPtr);
        }


        queueMutex.lock();
        //If more threads can be created
        if (numThreadsToCreate() > 0) {
            //Set this Worker to inactive so threads are not stolen prior to assigning threads tasks
            setInactive(this);

            //Launch threads to run tasks and gather results after they have finished
            farm(numThreadsToCreate());

            //Add self back to active list
            setActive(this);
        }
        queueMutex.unlock();

        //Recursively solve remaining tasks in inputQueue if any remain
        std::shared_ptr<Task<ProblemType, ResultType>> task;
        while ((task = getTaskInputQueue())) {
            ResultType solution = createWorker(task->getProblem(), false)->result;
            putResultOutputQueue(solution);
        }

        //Worker finished computation
        setInactive(this);

        //Wait for results to be placed in output queue if any have been stolen (non busy wait)
        if (outputQueue.size() != ps.size()) {
            std::unique_lock<std::mutex> lock(taskMutex);
            //Thread sleeps until outputQueue contains all required solutions
            conditionVariable.wait(lock, [&]{return outputQueue.size() == ps.size();});
        }

        //Get results from output queue
        std::vector<ResultType> res;
        while (!outputQueue.empty()) {
            res.push_back(getResultOutputQueue());
        }

        //Stores owner of stolen task
        Worker<ProblemType, ResultType>* owner;
        //Stores stolen task
        std::shared_ptr<Task<ProblemType, ResultType>> stolen;

        //While there is a valid task to steal...
        while (stealTask(owner, stolen)) {
            //...solve the task and write the results back to the outputQueue of the owner of the task
            owner->putResultOutputQueue(solve(stolen->getProblem()));
            owner->conditionVariable.notify_one();
        }

        //Combine the results and return
        return this->combine(res);
    }
}

/**
 * Create a number of workers running using threads and gather their results
 * @tparam ProblemType - type of problem to solve
 * @tparam ResultType - type of result of solving problem
 * @param numThreads - number of threads to create
 */
template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::farm(int numThreads) {
    //Records how many threads are currently running
    numActiveThreads += numThreads;

    //Assign tasks from input queue to new threads
    while (children.size() < numThreads) {
        std::shared_ptr<Task<ProblemType, ResultType>> task = getTaskInputQueue();
        children.push_back(createWorker(task->getProblem(), true));
    }

    //Gather results of thread computation
    while (!children.empty()) {
        std::shared_ptr<Worker<ProblemType, ResultType>> worker = children.front();
        //Join thread and get result of computation
        worker->thread.join();
        ResultType solveRes = worker->result;

        //Decrease number of active threads
        numActiveThreads--;

        //Store task on output queue
        putResultOutputQueue(solveRes);

        //Remove worker from list of children
        children.erase(children.begin());
    }
}

/**
 * Requests task from parent and returns whether one is available
 * @tparam ProblemType - type of problem
 * @tparam ResultType - type of result
 * @param taskOwner - owner of stolen task if one is available
 * @param task - stolen task if available
 * @return whether there is a task to steal
 */
template<typename ProblemType, typename ResultType>
bool Worker<ProblemType, ResultType>::stealTask(Worker<ProblemType, ResultType>*& taskOwner,
                                                std::shared_ptr<Task<ProblemType, ResultType>>& task) {
    std::lock_guard<std::mutex> lockGuard(Worker::activeMutex);
    taskOwner = nullptr;
    task = nullptr;

    //If no active workers, return false as no tasks to steal
    if (Worker::activeWorkers.size() == 0) return false;

    //Iterate through active workers
    for (auto it = Worker::activeWorkers.begin(); it != Worker::activeWorkers.end(); it++) {
        Worker<ProblemType, ResultType>* currentWorker = (*it);

        //Attempt to get a task from the worker
        task = currentWorker->getTaskInputQueue();
        //Keep track of the worker which requires the result of the task
        taskOwner = currentWorker;

        //If valid task has been retrieved, return true
        if (task != nullptr) {
            return true;
        }
    }

    //If no valid tasks return false
    return false;
}

/**
 * Gets the number of threads to create
 * (determined by number of free CPUs and threads currently running)
 * @return - number of threads current thread can create to run Worker objects
 */
template<typename ProblemType, typename ResultType>
int Worker<ProblemType, ResultType>::numThreadsToCreate() {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    int freeCPUs = numProcessors - numActiveThreads;

    //If size of task pool is greater than or equal to the number of free CPUs
    if (inputQueue.size() >= freeCPUs) {
        //Then create a new thread for each free CPU core
        return freeCPUs;
    } else {
        //Otherwise create one for each task
        return inputQueue.size();
    }
}

/**
 * Creates new worker to solve problem p and adds it to provided list of workers
 * @tparam ProblemType - type of problem to solve
 * @tparam ResultType - type of result
 * @param workers - list of workers to add worker to
 * @param p - problem to solve
 */
template<typename ProblemType, typename ResultType>
std::shared_ptr<Worker<ProblemType, ResultType>> Worker<ProblemType, ResultType>::createWorker(ProblemType p, bool newThread) {
    std::shared_ptr<Worker<ProblemType, ResultType>> worker =
            std::make_shared<Worker<ProblemType, ResultType>>(this->divide, this->combine, this->base, this->threshold, numActiveThreads, numProcessors);
    //Function to run ptr->solve and store result in ptr->result
    auto func = ([=]{worker->result = worker->solve(p);});

    //Run function either with new thread or existing thread
    if (newThread) {
        //Marks worker as active
        setActive(worker.get());
        //Runs worker with thread
        worker->thread = std::thread(func);
    } else {
        func();
    }

    return worker;
}

/**
 * Puts a task onto the inputQueue
 * @tparam ProblemType - problem type of task
 * @tparam ResultType - result type of task
 * @param t - task to enqueue
 */
template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::putTaskInputQueue(std::shared_ptr<Task<ProblemType, ResultType>> t) {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    inputQueue.push(t);
}

/**
 * Gets the task at the top of the input queue
 * @tparam ProblemType - type of problem
 * @tparam ResultType - type of result
 * @return task from input queue
 */
template<typename ProblemType, typename ResultType>
std::shared_ptr<Task<ProblemType, ResultType>> Worker<ProblemType, ResultType>::getTaskInputQueue() {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    //If inputQueue is empty return nullptr
    if (inputQueue.empty()) return nullptr;

    std::shared_ptr<Task<ProblemType, ResultType>> t = inputQueue.front();
    inputQueue.pop();
    return t;
}

/**
 * Puts a result on the outputQueue
 * @tparam ProblemType - type of problem
 * @tparam ResultType - type of result
 * @param result - result to put on outputQueue
 */
template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::putResultOutputQueue(ResultType result) {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    outputQueue.push(result);
}

/**
 * Gets a result from the outputQueue
 * @tparam ProblemType
 * @tparam ResultType
 * @return result at front of outputQueue
 */
template<typename ProblemType, typename ResultType>
ResultType Worker<ProblemType, ResultType>::getResultOutputQueue() {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    ResultType res = outputQueue.front();
    outputQueue.pop();
    return res;
}

/**
 * Add worker to active set of Worker objects
 * @tparam ProblemType - type of problem
 * @tparam ResultType - type of result
 * @param worker - to add
 */
template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::setActive(Worker<ProblemType, ResultType>* worker) {
    std::lock_guard<std::mutex> lockGuard(Worker::activeMutex);
    //If worker inactive (i.e. not in set), then add to active workers
    if (Worker::activeWorkers.find(worker) == Worker::activeWorkers.end()) {
        Worker::activeWorkers.insert(worker);
    }
}

/**
 * Remove Worker from active set of Worker objects
 * @tparam ProblemType - type of problem
 * @tparam ResultType - type of result
 * @param worker - to remove from set of active workers
 */
template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::setInactive(Worker<ProblemType, ResultType>* worker) {
    //If worker active (in set), then remove from active workers
    std::lock_guard<std::mutex> lockGuard(Worker::activeMutex);
    if (Worker::activeWorkers.find(worker) != Worker::activeWorkers.end()) {
        Worker::activeWorkers.erase(worker);
    }
}