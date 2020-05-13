#ifndef CS4204_P2_DC_MAIN_H
#define CS4204_P2_DC_MAIN_H

//Exit code if invalid argument entered
#define INVALID_ARG -1
//Number of repeat readings to perform
#define REPEAT_READINGS 3
//Maximum fibonacci to calculate
#define MAX_FIB 37
//Maximum size of list to sort
#define MAX_LIST_SIZE 1000000
//Amount to increase list size by each increment
#define LIST_INCREMENT 10000

#define MAX_QSORT_LIST_SIZE 10000
#define QSORT_LIST_INCREMENT 100

void runBenchmarks(unsigned int processors, bool runFib, bool runMerge, bool runQSort);
void fibBenchmark(unsigned int processors);
void mergeSortBenchmark(unsigned int processors);
void quickSortBenchmark(unsigned int processors);


#endif //CS4204_P2_DC_MAIN_H
