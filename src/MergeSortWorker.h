#ifndef CS4204_P2_DC_MERGESORTWORKER_H
#define CS4204_P2_DC_MERGESORTWORKER_H

#include <memory>
#include "Worker.h"
#include "List.h"

class MergeSortWorker : public Worker<List, List> {
public:

    MergeSortWorker() : Worker(
            [](const List& list) {
                std::vector<List> lists;
                //Calculates middle of list
                int m = list.low + (list.high - list.low) / 2;
                //Creates two new List objects for the left and right halves of the original list
                List listLeft(list.arr, list.low, m);
                List listRight(list.arr, m + 1, list.high);

                //Adds List objects to divides problem set
                lists.push_back(listLeft);
                lists.push_back(listRight);

                return lists;
            },
            [](std::vector<List> results) {
                int i, j, k;
                int l = results.at(0).low;
                int r = results.at(1).high;
                int m = l + (r - l) / 2;
                int n1 = m - l + 1;
                int n2 = r - m;

                //Allows vector of integers to be manipulated as an int[]
                int* arr = results.at(0).arr;

                /* create temp arrays */
                int L[n1], R[n2];

                /* Copy data to temp arrays L[] and R[] */
                for (i = 0; i < n1; i++)
                    L[i] = results.at(0).arr[l + i];
                for (j = 0; j < n2; j++)
                    R[j] = results.at(1).arr[m + 1 + j];

                /* Merge the temp arrays back into arr[l..r]*/
                i = 0; // Initial index of first subarray
                j = 0; // Initial index of second subarray
                k = l; // Initial index of merged subarray
                while (i < n1 && j < n2)
                {
                    if (L[i] <= R[j])
                    {
                        arr[k] = L[i];
                        i++;
                    }
                    else
                    {
                        arr[k] = R[j];
                        j++;
                    }
                    k++;
                }

                /* Copy the remaining elements of L[], if there
                are any */
                while (i < n1)
                {
                    arr[k] = L[i];
                    i++;
                    k++;
                }

                /* Copy the remaining elements of R[], if there
                are any */
                while (j < n2)
                {
                    arr[k] = R[j];
                    j++;
                    k++;
                }

                List list(arr, l, r);
                return list;
            },
            [](const List& list) {
                return list;
            },
            [](const List& list) {
                return list.low >= list.high;
            }
    ) {

    }
};


#endif //CS4204_P2_DC_MERGESORTWORKER_H
