#include <iostream>
#include "FibWorker.h"
#include "MergeSortWorker.h"
#include "QuickSortWorker.h"
#include <getopt.h>
#include <fstream>
#include "main.h"

/**
 * Main function from which to run program
 * @param argc - number of command line arguments
 * @param argv - provided command line arguments
 * @return 0 on success
 */
int main(int argc, char** argv) {
    //Defaults to the detected number of processors if -p flag is not provided
    unsigned int numProcessors = std::thread::hardware_concurrency();
    std::cout << "Number of processors on machine: " << numProcessors << std::endl;
    bool runFib = false;
    bool runMerge = false;
    bool runQSort = false;
    int ch;

    //Get command line arguments
    while ((ch = getopt(argc, argv, "p:fmq")) != -1)
    {
        switch (ch)
        {
            //Specify the number of processor cores to use
            case 'p':
                numProcessors = atoi(optarg);
                break;
            //Specify that fib benchmark should be run
            case 'f':
                runFib = true;
                break;
            //Specify that merge sort benchmark should be run
            case 'm':
                runMerge = true;
                break;
            //Specify that quick sort benchmark should be run
            case 'q':
                runQSort = true;
                break;
            default:
                //Usage message if invalid flag was entered
                std::cout << "Usage: ./dac -p <num processors> -f -m -q" << std::endl;
                std::cout << "-p, specify number of processors cores to use" << std::endl;
                std::cout << "-f, run fibonacci benchmark" << std::endl;
                std::cout << "-m, run merge sort benchmark" << std::endl;
                std::cout << "-q, run quick sort benchmark" << std::endl;
                exit(INVALID_ARG);
        }
    }

    //Run specified benchmarks
    runBenchmarks(numProcessors, runFib, runMerge, runQSort);
    return 0;
}

/**
 * Run each of the specified benchmarks
 * @param processors - number of processor cores to use
 * @param runFib - whether to run fibonacci benchmark
 * @param runMerge - whether to run mergesort benchmark
 * @param runQSort - whether to run quicksort benchmark
 */
void runBenchmarks(unsigned int processors, bool runFib, bool runMerge, bool runQSort) {
    //Run each benchmark if enabled:

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

/**
 * Benchmark for the fibonacci divide and conquer pattern
 * @param processors - number of CPU cores to use
 */
void fibBenchmark(unsigned int processors) {
    FibWorker fibWorker(processors);

    //Run for a number of repeat readings
    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../data/fib_" + std::to_string(processors) + "_" + std::to_string(run) + ".csv";
        std::ofstream outputFile(fileName);

        //Run fib(n) for n = 0 to n = MAX_FIB
        for (int n = 0; n <= MAX_FIB; n++) {
            //Calculate fib and record time taken
            auto startTime = std::chrono::high_resolution_clock::now();
            unsigned long int result = fibWorker.solve(n);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            //Write to file and print
            outputFile << n << ", " << duration.count() << std::endl;
            std::cout << "Fib(" << n << ") = " << result << ", CPUs = " << std::to_string(processors) << ", time = " << duration.count() << "ms" << std::endl;
        }
    }
}

/**
 * Benchmark for the merge sort divide and conquer pattern
 * @param processors - number of CPU cores to use
 */
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

            //Sort list and record time taken
            auto startTime = std::chrono::high_resolution_clock::now();
            mergeSortWorker.solve(List(arr, l, r));
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            //Write result to file
            outputFile << n << ", " << duration.count() << std::endl;

            //Print result to terminal
            std::cout << "mergesort(" << n << "), CPUs = " << std::to_string(processors) << ", time = " << duration.count() << "ms" << std::endl;
        }

        //Free memory
        free(arr);
    }
}

/**
 * Benchmark for the quick sort divide and conquer pattern
 * @param processors - number of CPU cores to use
 */
void quickSortBenchmark(unsigned int processors) {
    QuickSortWorker quickSortWorker(processors);

    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../data/qsort_" + std::to_string(processors) + "_" + std::to_string(run) + ".csv";
        std::ofstream outputFile(fileName);

        //Create int* to point to list of numbers to be sorted
        int* arr = (int*) malloc(1 * sizeof(int));

        for (int n = QSORT_LIST_INCREMENT; n <= MAX_QSORT_LIST_SIZE; n += QSORT_LIST_INCREMENT) {
            //Allocate memory for values to be sorted
            arr = (int*) realloc(arr, n * sizeof(int));
            int l = 0;
            int r = n - 1;

            //Assign values such that arr is reverse sort
            for (int i = 0, j = n; i < n; i++, j--) {
                arr[i] = j;
            }

            //Sort list and record time taken
            auto startTime = std::chrono::high_resolution_clock::now();
            quickSortWorker.solve(List(arr, l, r));
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            //Write result to file
            outputFile << n << ", " << duration.count() << std::endl;

            //Print result to terminal
            std::cout << "qsort(" << n << "), CPUs = " << std::to_string(processors) << ", time = " << duration.count() << "ms" << std::endl;
        }

        //Free memory
        free(arr);
    }
}