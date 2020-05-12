#ifndef CS4204_P2_DC_MAIN_H
#define CS4204_P2_DC_MAIN_H

#define INVALID_ARG -1
#define REPEAT_READINGS 5
#define MAX_FIB 37
#define MAX_LIST_SIZE 1000000
#define LIST_INCREMENT 10000

void runBenchmarks(unsigned int processors, bool runFib, bool runMerge, bool runQSort);
void fibBenchmark(unsigned int processors, std::set<unsigned int> coresToTest);
void mergeSortBenchmark(unsigned int processors, std::set<unsigned int> coresToTest);
void quickSortBenchmark(unsigned int processors, std::set<unsigned int> coresToTest);


#endif //CS4204_P2_DC_MAIN_H
