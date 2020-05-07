#include <iostream>
#include "FibSkeleton.h"

int FibSkeleton::solve(int p) {
    if (!threshold(p)) {
        std::vector<int> ps;
        ps = divide(p);
        std::vector<int> res(ps.size());
        for (int i = 0; i < ps.size(); i++) {
            res[i] = solve(ps[i]);
        }
        return combine(res);
    }
    return base(p);
}
