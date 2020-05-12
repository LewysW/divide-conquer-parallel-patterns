#ifndef CS4204_P2_DC_MAIN_H
#define CS4204_P2_DC_MAIN_H

#define INVALID_ARG -1
#define REPEAT_READINGS 3
#define MAX_FIB 37
#define MAX_LIST_SIZE 1000000
#define LIST_INCREMENT 10000

void runBenchmarks(unsigned int processors, bool runFib, bool runMerge, bool runQSort);
void fibBenchmark(unsigned int processors);
void mergeSortBenchmark(unsigned int processors);
void quickSortBenchmark(unsigned int processors);


#endif //CS4204_P2_DC_MAIN_H
