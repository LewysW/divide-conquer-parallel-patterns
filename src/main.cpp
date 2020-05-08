#include <iostream>
#include "FibWorker.h"
#include "MergeSortWorker.h"
#include "utility.h"

int main() {
    int n;
    std::cout << "Please enter an integer value: ";
    std::cin >> n;
    FibWorker fibWorker;
    std::cout << "Fib(" << n << ") = " << fibWorker.solve(n) << std::endl;
    std::cout << std::endl;

    int* arr = (int*) malloc(10 * sizeof(int));
    int l = 0;
    int r = 9;
    int num = 10;
    for (int i = 0; i < 10; i++) {
        arr[i] = num--;
    }

    std::cout << "Unsorted list: ";
    printArray(arr, 10);

    MergeSortWorker mergeSortWorker;
    mergeSortWorker.solve(List(arr, l, r));

    std::cout << "Sorted list: ";
    printArray(arr, 10);
    return 0;
}