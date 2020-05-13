#ifndef CS4204_P2_DC_QUICKSORTWORKER_H
#define CS4204_P2_DC_QUICKSORTWORKER_H

#include "Worker.h"
#include "List.h"

/**
 * Represents a worker whose purpose is to sort a list using quick sort
 */
class QuickSortWorker : public Worker<List, List> {
public:
    /**
     * Provided swap function to swap two numbers
     * @param a - pointer to first number
     * @param b - pointer to second number
     */
    static void swap(int* a, int* b)
    {
        int t = *a;
        *a = *b;
        *b = t;
    }

    /**
     * Provided quick sort partition function which is used to sort part of the original list
     * @param arr - list to sort
     * @param low - lower index of sublist to sort
     * @param high - upper index of sublist to sort
     */
    static int partition (int arr[], int low, int high)
    {
        int pivot = arr[high]; // pivot
        int i = (low - 1); // Index of smaller element

        for (int j = low; j <= high- 1; j++)
        {
            // If current element is smaller than the pivot
            if (arr[j] < pivot)
            {
                i++; // increment index of smaller element
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);
        return (i + 1);
    }

    /**
     * Constructor for QuickSortWorker
     * Initialises the following functions of the divide and conquer skeleton:
     * divide, combine, base, and threshold.
     * Each lambda function maps to a part of the provided sequential quick sort program
     * @param numProcessors
     */
    QuickSortWorker(const unsigned int numProcessors) : Worker(
            //Divide - sort the left and right sub lists before and after the partition pi
            [](const List& list) {
                std::vector<List> lists;
                int pi = QuickSortWorker::partition(list.arr, list.low, list.high);
                List listLeft(list.arr, list.low, pi - 1);
                List listRight(list.arr, pi + 1, list.high);

                //Divide problem in two by defining two List objects
                //storing the same array but each having different low and high indexes
                lists.push_back(listLeft);
                lists.push_back(listRight);

                return lists;
            },
            //Combine
            [](const std::vector<List>& results) {
                //In quick sort nothing happens at the combine stage
                //Simply return the whole list
                int low = results.at(0).low;
                int high = results.at(results.size() - 1).high;
                List list(results.at(0).arr, low, high);
                return list;
            },
            //Base
            [](const List& list) {
                return list;
            },
            //Threshold
            [](const List& list) {
                return list.low >= list.high;
            },
            //No active threads initially
            0,
            //Number of processors to run on
            numProcessors
    ) {}
};


#endif //CS4204_P2_DC_QUICKSORTWORKER_H
