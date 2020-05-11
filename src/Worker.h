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

    Worker<ProblemType, ResultType>(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold,
           const unsigned int numActiveThreads, const unsigned int numProcessors) :
                                                    DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold),
                                                    numActiveThreads(numActiveThreads), numProcessors(numProcessors) {}

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

#endif //CS4204_P2_DC_WORKER_H
