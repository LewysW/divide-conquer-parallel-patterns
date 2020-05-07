#ifndef CS4204_P2_DC_FIBSKELETON_H
#define CS4204_P2_DC_FIBSKELETON_H


#include "DCSkeleton.h"

class FibSkeleton : public DCSkeleton<int, int> {
public:
    /**
     * Constructor for FibSkeleton
     * Initialises the following functions of the divide and conquer skeleton:
     * divide, combine, base, and threshold
     */
    FibSkeleton() : DCSkeleton<int, int>(
            [](const int& p) {
                std::vector<int> children;
                children.push_back(p - 1);
                children.push_back(p - 2);
                return children;
            },
            [](std::vector<int> results) {
                return results[0] + results[1];
            },
            [](const int& p) {
                return 1;
            },
            [](const int& p) {
                return p < 2;
            }
    ) {

    }

    int solve(int n) override;
};


#endif //CS4204_P2_DC_FIBSKELETON_H
