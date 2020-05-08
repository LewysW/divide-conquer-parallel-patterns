#ifndef CS4204_P2_DC_QUICKSORTWORKER_H
#define CS4204_P2_DC_QUICKSORTWORKER_H

#include "Worker.h"
#include "List.h"

class QuickSortWorker : public Worker<List, List> {
public:
    static void swap(int* a, int* b)
    {
        int t = *a;
        *a = *b;
        *b = t;
    }

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

    QuickSortWorker() : Worker(
            //Divide
            [](const List& list) {
                std::vector<List> lists;
                int pi = QuickSortWorker::partition(list.arr, list.low, list.high);

                List listLeft(list.arr, list.low, pi - 1);
                List listRight(list.arr, pi + 1, list.high);

                lists.push_back(listLeft);
                lists.push_back(listRight);

                return lists;
            },
            //Combine
            [](std::vector<List> results) {
                //In quicksort nothing happens at the combine stage
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
            }
    ) {

    }
};


#endif //CS4204_P2_DC_QUICKSORTWORKER_H
