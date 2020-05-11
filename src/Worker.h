#ifndef CS4204_P2_DC_WORKER_H
#define CS4204_P2_DC_WORKER_H

#include <queue>
#include <thread>
#include <iostream>
#include "DCSkeleton.h"
#include "Task.h"
#include <mutex>
#include <condition_variable>
#include <future>
#include <set>
#include <random>

template <typename ProblemType, typename ResultType>
class Worker : public DCSkeleton<ProblemType, ResultType> {
public:
    //Workers performing computation sequentially
    static std::set<Worker<ProblemType, ResultType>*> activeWorkers;
    static std::mutex activeMutex;

    //Queue to store tasks to be solved
    std::queue<std::shared_ptr<Task<ProblemType, ResultType>>> inputQueue;

    //Queue to store tasks which have been solved
    std::queue<ResultType> outputQueue;

    //Mutex to lock queues
    std::mutex queueMutex;

    //Mutex to wait for completion of tasks
    std::mutex taskMutex;

    //Conditional variable to check if all results have been calculated
    std::condition_variable conditionVariable;

    //Vector to store child workers
    std::vector<std::shared_ptr<Worker<ProblemType, ResultType>>> children;

    //Number of active threads when this thread was created
    unsigned int numActiveThreads;

    //Stores created thread
    std::thread thread;

    //Allows us to get the return value of thread if this was run using a std::thread
    ResultType result;

    Worker<ProblemType, ResultType>(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold,
           const int numActiveThreads) :
                                                    DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold),
                                                    numActiveThreads(numActiveThreads) {}

    ResultType solve(ProblemType p);

    int numThreadsToCreate();

    std::shared_ptr<Worker<ProblemType, ResultType>> createWorker(ProblemType p, bool newThread);

    void putTaskInputQueue(std::shared_ptr<Task<ProblemType, ResultType>> t);

    std::shared_ptr<Task<ProblemType, ResultType>> getTaskInputQueue();

    void putResultOutputQueue(ResultType result);

    ResultType getResultOutputQueue();

    unsigned int getSize(std::queue<std::shared_ptr<Task<ProblemType, ResultType>>> queue);

    void farm(int numThreads);

    bool stealTask(Worker<ProblemType, ResultType>*& taskOwner,
            std::shared_ptr<Task<ProblemType, ResultType>>& task);

    void setActive(Worker<ProblemType, ResultType>* worker);
    void setInactive(Worker<ProblemType, ResultType>* worker);
};

//TODO - move implementation to cpp file

template <typename ProblemType, typename ResultType>
std::set<Worker<ProblemType, ResultType>*> Worker<ProblemType, ResultType>::activeWorkers = {};

template <typename ProblemType, typename ResultType>
std::mutex Worker<ProblemType, ResultType>::activeMutex;


//TODO - put code in functions

//TODO - clean up overall function to be similar to:
/**
 * if (threshold)
 *  return base
 * else
 *  if (threadsToCreate > 0)
 *      farm(threads)
 *
 *  solveRemainingProblem()
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

        //Want to avoid task stealing while assigning tasks to threads
        setInactive(this);

        //If more threads can be created
        if (numThreadsToCreate() > 0) {
            //Launch threads to run tasks and gather results after they have finished
            farm(numThreadsToCreate());
        }

        //Add self back to active list
        setActive(this);

        //Recursively solve remaining tasks in inputQueue if any remain
        std::shared_ptr<Task<ProblemType, ResultType>> task;
        while ((task = getTaskInputQueue())) {
            ResultType solution = createWorker(task->getProblem(), false)->result;
            putResultOutputQueue(solution);
        }

        //Worker finished computation
        setInactive(this);

        //Lock task mutex
        std::unique_lock lock(taskMutex);
        //Move on if results have been produced for every task
        while (outputQueue.size() != ps.size()) {
            //Condition variable sporadically wakes up to check loop condition
            //this technique avoids busy waiting
            conditionVariable.wait(lock);
        }

        //Get results from output queue
        std::vector<ResultType> res;
        while (!outputQueue.empty()) {
            res.push_back(getResultOutputQueue());
        }

        //TODO - steal tasks while there are tasks valid to steal

//        Worker<ProblemType, ResultType>* owner;
//        std::shared_ptr<Task<ProblemType, ResultType>> stolen;
//
//        //While there is a valid task to steal...
//        while (stealTask(owner, stolen)) {
//            //...solve the task and write the results back to the owner of the task
//            owner->putResultOutputQueue(solve(stolen->getProblem()));
//        }

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

    //Generate random index in activeWorkers
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0,  Worker::activeWorkers.size() - 1);
    int index = dist(mt);

    //Get iterator at index of that worker
    typename std::set<Worker<ProblemType, ResultType>*>::iterator it = Worker::activeWorkers.begin();
    std::advance(it, index);

    //repeat while we don't have a task
    int currentWorker = index;
    while (task == nullptr) {
        //If current worker to steal from is at same depth or lower in tree
        // (meaning they need to finish before the results of the our worker are useful)
        if ((*it)->numActiveThreads >= this->numActiveThreads) {
            //Attempt to get a task from the current worker
            task = (*it)->getTaskInputQueue();
            //Store current worker as it relies on the result of the task
            taskOwner = *it;
        } else {
            it++;
            currentWorker++;

            //If we have traversed entire data structure...
            if (currentWorker == index) {
                //... then there are no valid workers from which to steal tasks
                return false;
            } else if (it == Worker::activeWorkers.end()) {
                //Wrap around to start of data structure if we reached the end
                it = Worker::activeWorkers.begin();
                currentWorker = 0;
            }
        }
    }

    return true;
}



/**
 * Gets the number of threads to create
 * (determined by number of free CPUs and threads currently running)
 */
template<typename ProblemType, typename ResultType>
int Worker<ProblemType, ResultType>::numThreadsToCreate() {
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    unsigned int numCPUs = std::thread::hardware_concurrency();
    int freeCPUs = numCPUs - numActiveThreads;

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
            std::make_shared<Worker<ProblemType, ResultType>>(this->divide, this->combine, this->base, this->threshold, numActiveThreads);
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
    std::lock_guard<std::mutex> lockGuard(queueMutex);
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
    std::lock_guard<std::mutex> lockGuard(queueMutex);
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
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    return queue.size();
}

template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::putResultOutputQueue(ResultType result) {
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    outputQueue.push(result);
}

template<typename ProblemType, typename ResultType>
ResultType Worker<ProblemType, ResultType>::getResultOutputQueue() {
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    ResultType result = outputQueue.front();
    outputQueue.pop();
    return result;
}

template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::setActive(Worker<ProblemType, ResultType>* worker) {
    std::lock_guard<std::mutex> lockGuard(Worker::activeMutex);
    //If worker inactive, then add to active workers
    if (Worker::activeWorkers.find(worker) != Worker::activeWorkers.end()) {
        Worker::activeWorkers.insert(worker);
    }
}

template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::setInactive(Worker<ProblemType, ResultType>* worker) {
    //If worker active, then remove from active workers
    std::lock_guard<std::mutex> lockGuard(Worker::activeMutex);
    if (Worker::activeWorkers.find(worker) != Worker::activeWorkers.end()) {
        Worker::activeWorkers.erase(worker);
    }
}

#endif //CS4204_P2_DC_WORKER_H
