#include <iostream>
#include "FibWorker.h"
#include "MergeSortWorker.h"
#include "utility.h"
#include "QuickSortWorker.h"
#include <random>

int main() {
    //TODO WRITE BENCHMARKS

    //TODO - add parameter to Worker subclasses allow number of cores to be specified
    //Solves fibonacci(n)
    int n;
    std::cout << "Please enter an integer value: ";
    std::cin >> n;
    FibWorker fibWorker;
    std::cout << "Fib(" << n << ") = " << fibWorker.solve(n) << std::endl;
    std::cout << std::endl;

    int sizeOfList = 100;
    int* arr = (int*) malloc(sizeOfList * sizeof(int));
    int* arr2 = (int*) malloc(sizeOfList * sizeof(int));
    int l = 0;
    int r = sizeOfList - 1;
    //TODO - change to be reverse sorted list
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 100);

    for (int i = 0; i < sizeOfList; i++) {
        arr[i] = dist(mt);
        arr2[i] = dist(mt);
    }

    //Sorts a list using merge sort
    std::cout << "Merge Sort:" << std::endl;
    std::cout << "Unsorted list: ";
    printArray(arr, sizeOfList);

    MergeSortWorker mergeSortWorker;
    mergeSortWorker.solve(List(arr, l, r));

    std::cout << "Sorted list: ";
    printArray(arr, sizeOfList);
    std::cout << std::endl;

    //Sorts a list using quicksort
    std::cout << "Quick Sort:" << std::endl;
    std::cout << "Unsorted list: ";
    printArray(arr2, sizeOfList);

    QuickSortWorker quickSortWorker;
    quickSortWorker.solve(List(arr2, l, r));

    std::cout << "Sorted list: ";
    printArray(arr2, sizeOfList);

    return 0;
}