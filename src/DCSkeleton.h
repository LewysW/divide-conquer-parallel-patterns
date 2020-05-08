#ifndef CS4204_P2_DC_DCSKELETON_H
#define CS4204_P2_DC_DCSKELETON_H

#include <functional>
#include <utility>

/**BEGIN REFERENCE:
 Class adapted from https://dl.acm.org/doi/pdf/10.1145/3002125.3002128
**/
template<typename ProblemType, typename ResultType>
class DCSkeleton {
public:
    //Takes a problem to solve and divides it into multiple problems
    std::function<std::vector<ProblemType>(const ProblemType& p)> divide;
    //Takes the results of multiple problems and combines them into a single result
    std::function<ResultType(std::vector<ResultType> results)> combine;
    //Takes a problem and returns the base case result
    std::function<ResultType(const ProblemType& p)> base;
    //Returns whether the problem has passed some threshold indicating to return the base result
    std::function<bool (const ProblemType& p)> threshold;
    DCSkeleton(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
               const std::function<ResultType(std::vector<ResultType> results)> &combine,
               const std::function<ResultType(const ProblemType &)> &base,
               const std::function<bool(const ProblemType &)> &threshold) : divide(divide), combine(combine),
                                                                             base(base), threshold(threshold) {}
};

/**
 * END REFERENCE
 */

#endif //CS4204_P2_DC_DCSKELETON_H
