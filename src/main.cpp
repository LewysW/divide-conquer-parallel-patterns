#include <iostream>
#include "FibWorker.h"
#include "MergeSortWorker.h"
#include "utility.h"
#include "QuickSortWorker.h"
#include <random>
#include <getopt.h>
#include "main.h"

int main(int argc, char** argv) {
    unsigned int numProcessors = std::thread::hardware_concurrency();
    bool runFib = false;
    bool runMerge = false;
    bool runQSort = false;
    int ch;

    while ((ch = getopt(argc, argv, "p:fmq")) != -1)
    {
        switch (ch)
        {
            case 'p':
                numProcessors = atoi(optarg);
                break;
            case 'f':
                runFib = true;
                break;
            case 'm':
                runMerge = true;
                break;
            case 'q':
                runQSort = true;
                break;
            default:
                //std::cout << optarg << std::endl;
                std::cout << "Usage: ./dac -p <num processors> -f -m -q" << std::endl;
                std::cout << "-p, specify max number of processors to be run" << std::endl;
                std::cout << "-f, run fibonacci benchmark" << std::endl;
                std::cout << "-m, run merge sort benchmark" << std::endl;
                std::cout << "-q, run quick sort benchmark" << std::endl;
                exit(INVALID_ARG);
        }
    }

    runBenchmarks(numProcessors, runFib, runMerge, runQSort);
    //TODO WRITE BENCHMARKS
    return 0;
}

void runBenchmarks(unsigned int processors, bool runFib, bool runMerge, bool runQSort) {
    //TODO - add parameter to Worker subclasses allow number of cores to be specified
    //Solves fibonacci(n)
    int n;
    std::cout << "Please enter an integer value: ";
    std::cin >> n;
    FibWorker fibWorker(4);
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

    MergeSortWorker mergeSortWorker(4);
    mergeSortWorker.solve(List(arr, l, r));

    std::cout << "Sorted list: ";
    printArray(arr, sizeOfList);
    std::cout << std::endl;

    //Sorts a list using quicksort
    std::cout << "Quick Sort:" << std::endl;
    std::cout << "Unsorted list: ";
    printArray(arr2, sizeOfList);

    QuickSortWorker quickSortWorker(4);
    quickSortWorker.solve(List(arr2, l, r));

    std::cout << "Sorted list: ";
    printArray(arr2, sizeOfList);
}