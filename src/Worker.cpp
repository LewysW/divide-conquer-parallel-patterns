#include "Worker.h"
#include "List.h"

//TODO - move implementation to cpp file

template class Worker<int, int>;
template class Worker<List, List>;

template <typename ProblemType, typename ResultType>
std::set<Worker<ProblemType, ResultType>*> Worker<ProblemType, ResultType>::activeWorkers;

template <typename ProblemType, typename ResultType>
std::mutex Worker<ProblemType, ResultType>::activeMutex;


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

        //Wait for results to be placed in output queue (non busy wait)
        if (outputQueue.size() != ps.size()) {
            std::unique_lock<std::mutex> lock(taskMutex);
            conditionVariable.wait(lock, [&]{return outputQueue.size() == ps.size();});
        }

        //Get results from output queue
        std::vector<ResultType> res;
        while (!outputQueue.empty()) {
            res.push_back(getResultOutputQueue());
        }

        Worker<ProblemType, ResultType>* owner;
        std::shared_ptr<Task<ProblemType, ResultType>> stolen;

        //While there is a valid task to steal...
        while (stealTask(owner, stolen)) {
            //...solve the task and write the results back to the owner of the task
            owner->putResultOutputQueue(solve(stolen->getProblem()));
            owner->conditionVariable.notify_one();
        }

        //Combine the results and return
        return this->combine(res);
    }
}

template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::farm(int numThreads) {
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

    if (Worker::activeWorkers.size() == 0) return false;

    //Iterate through active workers
    for (auto it = Worker::activeWorkers.begin(); it != Worker::activeWorkers.end(); it++) {
        Worker<ProblemType, ResultType>* currentWorker = (*it);

        //If thread at same level in tree or deeper
        if (currentWorker->numActiveThreads == this->numActiveThreads) {
            //Attempt to get a task from the worker
            task = currentWorker->getTaskInputQueue();
            //Keep track of the worker which requires the result of the task
            taskOwner = currentWorker;

            //If valid task has been retrieved, return true
            if (task != nullptr) {
                return true;
            }
        }
    }

    //If no valid tasks return false
    return false;
}

/**
 * Gets the number of threads to create
 * (determined by number of free CPUs and threads currently running)
 */
template<typename ProblemType, typename ResultType>
int Worker<ProblemType, ResultType>::numThreadsToCreate() {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    int freeCPUs = numProcessors - numActiveThreads;

    //If size of task pool is greater than or equal to the number of free CPUS
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
 * Puts a task onto a task pool queue
 * @tparam ProblemType - problem type of task
 * @tparam ResultType - result type of task
 * @param queue - queue to enqueue task on
 * @param t - task to enqueue
 */
template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::putTaskInputQueue(std::shared_ptr<Task<ProblemType, ResultType>> t) {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    inputQueue.push(t);
}

/**
 * Gets the task at the top of a task pool queue
 * @tparam ProblemType - type of problem
 * @tparam ResultType - type of result
 * @param queue - queue to pop task from
 * @return task from queue
 */
template<typename ProblemType, typename ResultType>
std::shared_ptr<Task<ProblemType, ResultType>> Worker<ProblemType, ResultType>::getTaskInputQueue() {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    if (inputQueue.empty()) return nullptr;

    std::shared_ptr<Task<ProblemType, ResultType>> t = inputQueue.front();
    inputQueue.pop();
    return t;
}

/**
 * Gets the size of a given queue
 * @tparam ProblemType - type of problem
 * @tparam ResultType - type of result
 * @param queue - to get the size of
 * @return size of queue
 */
template<typename ProblemType, typename ResultType>
unsigned int Worker<ProblemType, ResultType>::getSize(std::queue<std::shared_ptr<Task<ProblemType, ResultType>>> queue) {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    return queue.size();
}

template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::putResultOutputQueue(ResultType result) {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    outputQueue.push(result);
}

template<typename ProblemType, typename ResultType>
ResultType Worker<ProblemType, ResultType>::getResultOutputQueue() {
    std::lock_guard<std::recursive_mutex> lockGuard(queueMutex);
    ResultType result = outputQueue.front();
    outputQueue.pop();
    return result;
}

template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::setActive(Worker<ProblemType, ResultType>* worker) {
    std::lock_guard<std::mutex> lockGuard(Worker::activeMutex);
    //If worker inactive (i.e. not in set), then add to active workers
    if (Worker::activeWorkers.find(worker) == Worker::activeWorkers.end()) {
        Worker::activeWorkers.insert(worker);
    }
}

template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::setInactive(Worker<ProblemType, ResultType>* worker) {
    //If worker active (in set), then remove from active workers
    std::lock_guard<std::mutex> lockGuard(Worker::activeMutex);
    if (Worker::activeWorkers.find(worker) != Worker::activeWorkers.end()) {
        Worker::activeWorkers.erase(worker);
    }
}