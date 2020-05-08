#ifndef CS4204_P2_DC_LIST_H
#define CS4204_P2_DC_LIST_H


#include <algorithm>
#include <utility>
#include <vector>

class List {
public:
    int* arr;
    int low;
    int high;

    List() = default;
    List(int* arr, int low, int high) : arr(arr), low(low), high(high) {}
};


#endif //CS4204_P2_DC_LIST_H
