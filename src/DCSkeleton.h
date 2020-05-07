#ifndef CS4204_P2_DC_DCSKELETON_H
#define CS4204_P2_DC_DCSKELETON_H


#include <vector>
#include <bits/shared_ptr.h>
#include <queue>
#include <functional>
#include "Task.h"

class DCSkeleton {
private:
    std::queue<std::shared_ptr<Task>> taskQueue;
    std::shared_ptr<DCSkeleton> parent;
    std::vector<std::shared_ptr<DCSkeleton>> children;

public:
    virtual std::vector<std::shared_ptr<Task>> divide(std::shared_ptr<Task> task) = 0;
    virtual std::shared_ptr<Task> combine(std::vector<std::shared_ptr<Task>> tasks) = 0;
    virtual bool base(std::shared_ptr<Task> task) = 0;
    virtual bool threshold(std::shared_ptr<Task> task) = 0;

    virtual
};


#endif //CS4204_P2_DC_DCSKELETON_H
