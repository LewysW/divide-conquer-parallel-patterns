#ifndef CS4204_P2_DC_LIST_H
#define CS4204_P2_DC_LIST_H


#include <algorithm>
#include <utility>
#include <vector>

/**
 * Wrapper List class to act as the problem to solve for merge sort and quick sort.
 */
class List {
public:
    //List to sort
    int* arr;
    //lower bound position in list to sort
    int low;
    //Upper bound position in list to sort
    int high;

    List() = default;
    List(int* arr, int low, int high) : arr(arr), low(low), high(high) {}
};


#endif //CS4204_P2_DC_LIST_H
