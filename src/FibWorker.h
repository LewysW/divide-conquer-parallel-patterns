#ifndef CS4204_P2_DC_FIBWORKER_H
#define CS4204_P2_DC_FIBWORKER_H

#include "Worker.h"

/**
 * Represents a worker whose purpose to find the nth number in the fibonacci sequence
 */
class FibWorker : public Worker<int, int> {
public:
    /**
     * Constructor for FibWorker
     * Initialises the following functions of the divide and conquer skeleton:
     * divide, combine, base, and threshold.
     * Each lambda function maps to a part of the provided sequential fibonacci program
     * @param numProcessors - number of CPU cores to use
     */
    FibWorker(const unsigned int numProcessors) : Worker(
            //Divide
            [](const int& p) {
                std::vector<int> children;
                children.push_back(p - 1);
                children.push_back(p - 2);
                return children;
            },
            //Combine
            [](std::vector<int> results) {
                return results[0] + results[1];
            },
            //Base
            [](const int& p) {
                return 1;
            },
            //Threshold
            [](const int& p) {
                return p < 2;
            },
            //Number of existing threads upon creation of this worker
            0,
            //Number of processors to run on
            numProcessors
    ) {}
};


#endif //CS4204_P2_DC_FIBWORKER_H
