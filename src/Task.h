#ifndef CS4204_P2_DC_TASK_H
#define CS4204_P2_DC_TASK_H

template <typename Value, typename Result>
class Task {
    const Value& val;
    const Result& res;

public:
    Task(const Value &val) : val(val) {}
};


#endif //CS4204_P2_DC_TASK_H
