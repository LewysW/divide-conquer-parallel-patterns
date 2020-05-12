#ifndef CS4204_P2_DC_TASK_H
#define CS4204_P2_DC_TASK_H

#include <memory>
#include <vector>

/**
 * Class to represent a unit of work in the system
 */
template <typename Problem, typename Result>
class Task {
private:
    //Stores problem to be solved
    Problem problem;
    //Stores result of solving problem
    Result result;

public:
    /**
     * Constructor for Task
     * @param problem - to solve
     */
    explicit Task(Problem problem) : problem(problem) {}

    /**
     * Constructor for Task
     * @param result - of solving problem
     */
    explicit Task(Problem problem, Result result) : result(result) {}

    /**
     * Getter for problem
     * @return problem to be solved
     */
    Problem getProblem() const {
        return problem;
    }
};


#endif //CS4204_P2_DC_TASK_H
