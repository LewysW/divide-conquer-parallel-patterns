#ifndef CS4204_P2_DC_TASK_H
#define CS4204_P2_DC_TASK_H

#include <memory>
#include <vector>

template <typename Problem, typename Result>
class Task {
private:
    //Stores problem and result of task
    Problem problem;
    Result result;

public:
    explicit Task(Problem problem) : problem(problem) {}

    explicit Task(Problem problem, Result result) : result(result) {}

    Problem getProblem() const {
        return problem;
    }
};


#endif //CS4204_P2_DC_TASK_H
