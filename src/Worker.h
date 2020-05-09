#ifndef CS4204_P2_DC_WORKER_H
#define CS4204_P2_DC_WORKER_H

#include <queue>
#include <thread>
#include <iostream>
#include "DCSkeleton.h"
#include "Task.h"

template <typename ProblemType, typename ResultType>
class Worker : public DCSkeleton<ProblemType, ResultType> {
private:
    //Queue to store tasks
    std::queue<std::shared_ptr<Task<ProblemType, ResultType>>> taskPool;

    //Vector to store child workers
    std::vector<Worker<ProblemType, ResultType>> children;

    //Processor count
    const unsigned int NUM_CPUS = std::thread::hardware_concurrency();

    //Number of active threads when this thread was created
    unsigned int numActiveThreads;

    //Parent pointer
    std::shared_ptr<Worker<ProblemType, ResultType>> parent;

public:
    Worker(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold) : DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold),
                                         numActiveThreads(NUM_CPUS) {}

    Worker(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold,
           const int numActiveThreads) : DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold),
                                                    numActiveThreads(numActiveThreads) {}

    ResultType solve(ProblemType p);

    int getNumFreeThreads();
};

template<typename ProblemType, typename ResultType>
ResultType Worker<ProblemType, ResultType>::solve(ProblemType p) {
    //TODO - add p to taskpool
    //If base condition
    if (this->threshold(p)) {
        //Return base solution
        return this->base(p);
    } else {
        //Otherwise split problem p into problems ps
        std::vector<ProblemType> ps;
        ps = this->divide(p);

        //TODO - add ps tasks as children of task p
        //TODO - add ps to task pool
        //TODO - need to spawn new threads at this stage if we have free CPU cores
        //TODO - assign each created thread an item from the task pool
        //TODO - remove item from task pool
        //TODO - create child worker and add to children of this worker
        //Use divide and conquer to solve each problem in ps
        std::vector<ProblemType> res(ps.size());
        for (int i = 0; i < ps.size(); i++) {
            res[i] = solve(ps[i]);
        }

        //Combine the results
        return this->combine(res);
    }
}

/**
 * Gets the number of threads a child thread can create
 */
template<typename ProblemType, typename ResultType>
int Worker<ProblemType, ResultType>::getNumFreeThreads() {
    int freeCPUs = NUM_CPUS - numActiveThreads;

    //If size of taskpool is greater than or equal to the number of free CPUS
    if (taskPool.size() >= freeCPUs) {
        //Then no more cores are available
        return 0;
    } else {
        //Otherwise return the remaining number of cores not in use
        return freeCPUs - taskPool.size();
    }
}

#endif //CS4204_P2_DC_WORKER_H
