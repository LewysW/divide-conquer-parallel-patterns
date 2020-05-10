#include <iostream>
#include "FibWorker.h"
#include "MergeSortWorker.h"
#include "utility.h"
#include "QuickSortWorker.h"

int main() {
    //TODO - add parameter to Worker subclasses allow number of cores to be specified
    //TODO - add sequential functions as attributes to run instead of creating workers when no threads are available
    //Solves fibonacci(n)
    int n;
    std::cout << "Please enter an integer value: ";
    std::cin >> n;
    FibWorker fibWorker;
    std::cout << "Fib(" << n << ") = " << fibWorker.solve(n) << std::endl;
    std::cout << std::endl;

    int* arr = (int*) malloc(10 * sizeof(int));
    int* arr2 = (int*) malloc(10 * sizeof(int));
    int l = 0;
    int r = 9;
    int num = 10;
    for (int i = 0; i < 10; i++) {
        arr[i] = num * 5 % 12;
        arr2[i] = num-- * 7 % 16;
    }

    //Sorts a list using merge sort
    std::cout << "Merge Sort:" << std::endl;
    std::cout << "Unsorted list: ";
    printArray(arr, 10);

    MergeSortWorker mergeSortWorker;
    mergeSortWorker.solve(List(arr, l, r));

    std::cout << "Sorted list: ";
    printArray(arr, 10);
    std::cout << std::endl;

    //Sorts a list using quicksort
    std::cout << "Quick Sort:" << std::endl;
    std::cout << "Unsorted list: ";
    printArray(arr2, 10);

    QuickSortWorker quickSortWorker;
    quickSortWorker.solve(List(arr2, l, r));

    std::cout << "Sorted list: ";
    printArray(arr2, 10);

    return 0;
}