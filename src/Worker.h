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

template <typename ProblemType, typename ResultType>
class Worker : public DCSkeleton<ProblemType, ResultType> {
public:
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

    //Processor cores not in use
    const unsigned int numCPUs = std::thread::hardware_concurrency();

    //Number of active threads when this thread was created
    unsigned int numActiveThreads;

    //Parent pointer if worker is a child
    Worker<ProblemType, ResultType>* parent;

    //Stores created thread
    std::thread thread;

    //Allows us to get the return value of thread if this was run using a std::thread
    std::future<ResultType> solution;

    Worker<ProblemType, ResultType>(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold,
           const int numActiveThreads) : DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold),
                                                    numActiveThreads(numActiveThreads) {}

    ResultType solve(ProblemType p);

    int numThreadsToCreate();

    void createWorker(std::vector<std::shared_ptr<Worker<ProblemType, ResultType>>> workers, ProblemType p);

    void putTask(std::shared_ptr<Task<ProblemType, ResultType>> t);

    std::shared_ptr<Task<ProblemType, ResultType>> getTask();

    void putResult(ResultType result);

    ResultType getResult();

    unsigned int getSize(std::queue<std::shared_ptr<Task<ProblemType, ResultType>>> queue);
};

//TODO - put code in functions
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

        //Adds tasks to input queue
        for (int i = 0; i < ps.size(); i++) {
            //TODO - simplify by passing task to putTask as parameter and making shared_ptr inside function
            //TODO - perhaps get rid of functions entirely and have code for queues in this function
            //TODO - maybe also just name functions e.g. putTaskOnInputQueue so it reads better
            std::shared_ptr<Task<ProblemType, ResultType>> taskPtr = std::make_shared<Task<ProblemType, ResultType>>(Task<ProblemType, ResultType>(ps[i]));
            putTask(taskPtr);
        }
        //Determine number of threads to run
        unsigned int threadsToCreate = numThreadsToCreate();

        //Assign tasks from input queue to new threads
        while (children.size() < threadsToCreate) {
            std::shared_ptr<Task<ProblemType, ResultType>> task = getTask();
            numActiveThreads++;
            createWorker(children, task->getProblem());
        }

        //Gather results of thread computation
        while (!children.empty()) {
            //Join thread and get result of computation
            children.front()->thread.join();
            ProblemType problemType;
            ResultType result = children.front()->solution.get();

            //Decrease number of active threads
            numActiveThreads--;

            //Store task on output queue
            putResult(result);

            //Remove worker from list of children
            children.erase(children.begin());
        }

        //Solve remaining tasks in inputQueue
        while (!inputQueue.empty()) {
            putResult(solve(getTask()->getProblem()));
        }

//        //Lock task mutex
//        std::unique_lock lock(taskMutex);
//
//        //Execution halts until outputQueue is the size of the problem set ps
//        //i.e. every problem in the problem set has produced a result
//        conditionVariable.wait(lock, getSize(outputQueue) == ps.size());

        std::vector<ResultType> res;

        //Get results from output queue
        while (!outputQueue.empty()) {
            res.push_back(getResult());
        }

        //Combine the results
        return this->combine(res);
    }
}

/**
 * Gets the number of threads to create
 * (determined by number of free CPUs and threads currently running)
 */
template<typename ProblemType, typename ResultType>
int Worker<ProblemType, ResultType>::numThreadsToCreate() {
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    int freeCPUs = numCPUs - numActiveThreads;

    //If size of task pool is greater than or equal to the number of free CPUS
    if (inputQueue.size() >= freeCPUs) {
        //Then no more cores are available
        return 0;
    } else {
        //Otherwise return the remaining number of cores not in use
        return freeCPUs - inputQueue.size();
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
void Worker<ProblemType, ResultType>::createWorker(std::vector<std::shared_ptr<Worker<ProblemType, ResultType>>> workers, ProblemType p) {
    unsigned int activeThreads = numThreadsToCreate() + numActiveThreads;
    workers.push_back(std::make_shared<Worker<ProblemType, ResultType>>(this->divide, this->combine, this->base, this->threshold, activeThreads));
    std::shared_ptr<Worker<ProblemType, ResultType>> ptr = workers.back();
    ptr->parent = this;
    std::promise<ResultType> promise;
    ptr->thread = std::thread(&Worker::solve, ptr, p);
    ptr->solution = promise.get_future();
}

/**
 * Puts a task onto a task pool queue
 * @tparam ProblemType - problem type of task
 * @tparam ResultType - result type of task
 * @param queue - queue to enqueue task on
 * @param t - task to enqueue
 */
template<typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::putTask(std::shared_ptr<Task<ProblemType, ResultType>> t) {
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
std::shared_ptr<Task<ProblemType, ResultType>> Worker<ProblemType, ResultType>::getTask() {
    std::lock_guard<std::mutex> lockGuard(queueMutex);
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
void Worker<ProblemType, ResultType>::putResult(ResultType result) {
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    outputQueue.push(result);
}

template<typename ProblemType, typename ResultType>
ResultType Worker<ProblemType, ResultType>::getResult() {
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    ResultType result = outputQueue.front();
    outputQueue.pop();
    return result;
}


#endif //CS4204_P2_DC_WORKER_H
