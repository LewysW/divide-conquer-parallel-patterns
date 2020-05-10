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
    ResultType result;

    Worker<ProblemType, ResultType>(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold,
           const int numActiveThreads) : DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold),
                                                    numActiveThreads(numActiveThreads) {}

    ResultType solve(ProblemType p);

    int numThreadsToCreate();

    std::shared_ptr<Worker<ProblemType, ResultType>> createWorker(ProblemType p);

    void putTaskInputQueue(std::shared_ptr<Task<ProblemType, ResultType>> t);

    std::shared_ptr<Task<ProblemType, ResultType>> getTaskInputQueue();

    void putResultOutputQueue(ResultType result);

    ResultType getResultOutputQueue();

    unsigned int getSize(std::queue<std::shared_ptr<Task<ProblemType, ResultType>>> queue);
};

//TODO - move implementation to cpp file

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
    std::cout << "Problem: " << p << std::endl;
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
            std::shared_ptr<Task<ProblemType, ResultType>> taskPtr = std::make_shared<Task<ProblemType, ResultType>>(Task<ProblemType, ResultType>(ps[i]));
            putTaskInputQueue(taskPtr);
        };
        //Determine number of threads to run
        unsigned int threadsToCreate = numThreadsToCreate();
        numActiveThreads += threadsToCreate;

        //Assign tasks from input queue to new threads
        while (children.size() < threadsToCreate) {
            std::shared_ptr<Task<ProblemType, ResultType>> task = getTaskInputQueue();
            children.push_back(createWorker(task->getProblem()));
        }

        //Gather results of thread computation
        while (!children.empty()) {
            //Join thread and get result of computation
            children.front()->thread.join();
            ResultType result = children.front()->result;

            //Decrease number of active threads
            numActiveThreads--;

            //Store task on output queue
            putResultOutputQueue(result);

            //Remove worker from list of children
            children.erase(children.begin());
        }

        //Solve remaining tasks in inputQueue
        while (!inputQueue.empty()) {
            putResultOutputQueue(solve(getTaskInputQueue()->getProblem()));
        }

        //Lock task mutex
        std::unique_lock lock(taskMutex);

        //Execution halts until outputQueue is the size of the problem set ps
        //i.e. every problem in the problem set has produced a result
        while (outputQueue.size() != ps.size()) {
            conditionVariable.wait(lock);
        }

        std::vector<ResultType> res;

        //Get results from output queue
        while (!outputQueue.empty()) {
            res.push_back(getResultOutputQueue());
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
std::shared_ptr<Worker<ProblemType, ResultType>> Worker<ProblemType, ResultType>::createWorker(ProblemType p) {
    std::shared_ptr<Worker<ProblemType, ResultType>> worker =
            std::make_shared<Worker<ProblemType, ResultType>>(this->divide, this->combine, this->base, this->threshold, numActiveThreads);
    worker->parent = this;

    //Run thread on ptr->solve and store result in ptr->result
    worker->thread = std::thread([=]{worker->result = worker->solve(p);});
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


#endif //CS4204_P2_DC_WORKER_H
