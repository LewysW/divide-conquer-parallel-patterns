#include <iostream>
#include "FibWorker.h"
#include "MergeSortWorker.h"
#include "utility.h"
#include "QuickSortWorker.h"
#include <random>
#include <getopt.h>
#include <fstream>
#include "main.h"

int main(int argc, char** argv) {
    unsigned int numProcessors = std::thread::hardware_concurrency();
    std::cout << "Number of processors on machine: " << numProcessors << std::endl;
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
    return 0;
}

void runBenchmarks(unsigned int processors, bool runFib, bool runMerge, bool runQSort) {
    //Run each benchmark if enabled

    if (runFib) {
        fibBenchmark(processors);
    }

    if (runMerge) {
        mergeSortBenchmark(processors);
    }

    if (runQSort) {
        quickSortBenchmark(processors);
    }
}

void fibBenchmark(unsigned int processors) {
    FibWorker fibWorker(processors);

    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../data/fib_" + std::to_string(processors) + "_" + std::to_string(run) + ".csv";
        std::ofstream outputFile(fileName);

        for (int n = 0; n <= MAX_FIB; n++) {
            auto startTime = std::chrono::high_resolution_clock::now();
            unsigned long int result = fibWorker.solve(n);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            outputFile << n << ", " << duration.count() << std::endl;
            std::cout << "Fib(" << n << ") = " << result << ", CPUs = " << std::to_string(processors) << ", time = " << duration.count() << "ms" << std::endl;
        }
    }
}

void mergeSortBenchmark(unsigned int processors) {
    MergeSortWorker mergeSortWorker(processors);

    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../data/merge_" + std::to_string(processors) + "_" + std::to_string(run) + ".csv";
        std::ofstream outputFile(fileName);

        //Create int* to point to list of numbers to be sorted
        int* arr = (int*) malloc(1 * sizeof(int));

        for (int n = LIST_INCREMENT; n <= MAX_LIST_SIZE; n += LIST_INCREMENT) {
            //Allocate memory for values to be sorted
            arr = (int*) realloc(arr, n * sizeof(int));
            int l = 0;
            int r = n - 1;

            //Assign values such that arr is reverse sort
            for (int i = 0, j = n; i < n; i++, j--) {
                arr[i] = j;
            }

            //Print unsorted list
            //std::cout << "\nUnsorted list: " << std::endl;
           //printArray(arr, n);

            //Sort list and record time taken
            auto startTime = std::chrono::high_resolution_clock::now();
            mergeSortWorker.solve(List(arr, l, r));
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            //Write result to file
            outputFile << n << ", " << duration.count() << std::endl;

            //Print sorted list
            //std::cout << "Sorted list: " << std::endl;
            //printArray(arr, n);

            //Print result to terminal
            std::cout << "mergesort(" << n << "), CPUs = " << std::to_string(processors) << ", time = " << duration.count() << "ms" << std::endl;
        }

        //Free memory
        free(arr);
    }
}

void quickSortBenchmark(unsigned int processors) {
    QuickSortWorker quickSortWorker(processors);

    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../data/qsort_" + std::to_string(processors) + "_" + std::to_string(run) + ".csv";
        std::ofstream outputFile(fileName);

        //Create int* to point to list of numbers to be sorted
        int* arr = (int*) malloc(1 * sizeof(int));

        for (int n = LIST_INCREMENT; n <= MAX_LIST_SIZE; n += LIST_INCREMENT) {
            //Allocate memory for values to be sorted
            arr = (int*) realloc(arr, n * sizeof(int));
            int l = 0;
            int r = n - 1;

            //Assign values such that arr is reverse sort
            for (int i = 0, j = n; i < n; i++, j--) {
                arr[i] = j;
            }

            //Print unsorted list
//                std::cout << "\nUnsorted list: " << std::endl;
//                printArray(arr, n);

            //Sort list and record time taken
            auto startTime = std::chrono::high_resolution_clock::now();
            quickSortWorker.solve(List(arr, l, r));
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            //Write result to file
            outputFile << n << ", " << duration.count() << std::endl;

            //Print sorted list
//                std::cout << "Sorted list: " << std::endl;
//                printArray(arr, n);

            //Print result to terminal
            std::cout << "qsort(" << n << "), CPUs = " << std::to_string(processors) << ", time = " << duration.count() << "ms" << std::endl;
        }

        //Free memory
        free(arr);
    }
}