#ifndef CS4204_P2_DC_WORKER_H
#define CS4204_P2_DC_WORKER_H

#include "DCSkeleton.h"

template <typename ProblemType, typename ResultType>
class Worker : public DCSkeleton<ProblemType, ResultType> {
public:
    Worker(const std::function<std::vector<ProblemType>(const ProblemType &)> &divide,
           const std::function<ResultType(std::vector<ResultType>)> &combine,
           const std::function<ResultType(const ProblemType &)> &base,
           const std::function<bool(const ProblemType &)> &threshold) : DCSkeleton<ProblemType, ResultType>(divide, combine, base, threshold) {}

    ResultType solve(ProblemType p);
};

template<typename ProblemType, typename ResultType>
ResultType Worker<ProblemType, ResultType>::solve(ProblemType p) {
    if (!this->threshold(p)) {
        std::vector<ProblemType> ps;
        ps = this->divide(p);
        std::vector<ProblemType> res(ps.size());
        for (int i = 0; i < ps.size(); i++) {
            res.insert(res.begin() + i, solve(ps[i]));
        }
        return this->combine(res);
    }
    return this->base(p);
}

#endif //CS4204_P2_DC_WORKER_H
