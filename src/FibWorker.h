#ifndef CS4204_P2_DC_FIBWORKER_H
#define CS4204_P2_DC_FIBWORKER_H

#include "Worker.h"

class FibWorker : public Worker<int, int> {
public:
    /**
     * Constructor for FibSkeleton
     * Initialises the following functions of the divide and conquer skeleton:
     * divide, combine, base, and threshold
     */
    FibWorker() : Worker(
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
            }
    ) {}
};


#endif //CS4204_P2_DC_FIBWORKER_H
