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
    //Stores child tasks, the result of which this task depends
    std::vector<std::shared_ptr<Task>> children;
    //Marks if task has been solved
    bool solved = false;

public:
    Task(Problem problem) : problem(problem) {}

    void addChild(std::shared_ptr<Task> task) {
        children.push_back(task);
    }

    Problem getProblem() const {
        return problem;
    }

    Result getResult() const {
        return result;
    }

    const std::vector<std::shared_ptr<Task>> &getChildren() const {
        return children;
    }

    bool isSolved() const {
        return solved;
    }
};


#endif //CS4204_P2_DC_TASK_H
