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

/**
 * Code run by thread to solve given problem
 * @tparam ProblemType
 * @tparam ResultType
 */
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
    std::recursive_mutex queueMutex;

    //Mutex to wait for completion of tasks
    std::mutex taskMutex;

    //Conditional variable to check if all results have been calculated
    std::condition_variable conditionVariable;

    //Vector to store child workers
    std::vector<std::shared_ptr<Worker<ProblemType, ResultType>>> children;

    //Number of active threads when this thread was created
    unsigned int numActiveThreads;

    //Number of processors to run worker with
    unsigned int numProcessors;

    //Stores created thread
    std::thread thread;

    //Allows us to get the return value of thread if this was run using a std::thread
    ResultType result;

    //Constructor for Worker
    Worker<ProblemType, ResultType>(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold,
           const unsigned int numActiveThreads, const unsigned int numProcessors) :
                                                    DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold),
                                                    numActiveThreads(numActiveThreads), numProcessors(numProcessors) {
    }

    //Solves given problem
    ResultType solve(ProblemType p);

    //Returns number of threads a thread can create
    int numThreadsToCreate();

    //Creates a worker (either to be run by a new thread or sequentially)
    std::shared_ptr<Worker<ProblemType, ResultType>> createWorker(ProblemType p, bool newThread);

    //Add a task to the input queue
    void putTaskInputQueue(std::shared_ptr<Task<ProblemType, ResultType>> t);

    //Get a task from the input queue
    std::shared_ptr<Task<ProblemType, ResultType>> getTaskInputQueue();

    //Add a result to the output queue
    void putResultOutputQueue(ResultType result);

    //Get a result from the output queue
    ResultType getResultOutputQueue();

    //Create a number of threads, assign them work, and reap the results
    void farm(int numThreads);

    //Steal a task from another thread
    bool stealTask(Worker<ProblemType, ResultType>*& taskOwner,
            std::shared_ptr<Task<ProblemType, ResultType>>& task);

    //Add worker to active data structure
    void setActive(Worker<ProblemType, ResultType>* worker);

    //Remove worker from active data structure
    void setInactive(Worker<ProblemType, ResultType>* worker);
};

#endif //CS4204_P2_DC_WORKER_H
