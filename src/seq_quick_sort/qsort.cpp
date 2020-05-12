/* C implementation QuickSort */
#include<stdlib.h>
#include<stdio.h>
#include "../main.h"
#include "../../../../../../../usr/include/c++/7/string"
#include "../../../../../../../usr/include/c++/7/fstream"
#include "../../../../../../../usr/include/c++/7/chrono"
#include "../../../../../../../usr/include/c++/7/iostream"

// A utility function to swap two elements 
void swap(int* a, int* b) 
{ 
	int t = *a; 
	*a = *b; 
	*b = t; 
} 

/* This function takes last element as pivot, places 
the pivot element at its correct position in sorted 
	array, and places all smaller (smaller than pivot) 
to left of pivot and all greater elements to right 
of pivot */
int partition (int arr[], int low, int high) 
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

/* The main function that implements QuickSort 
arr[] --> Array to be sorted, 
low --> Starting index, 
high --> Ending index */
void quickSort(int arr[], int low, int high) 
{ 
	if (low < high) 
	{ 
		/* pi is partitioning index, arr[p] is now 
		at right place */
		int pi = partition(arr, low, high); 

		// Separately sort elements before 
		// partition and after partition 
		quickSort(arr, low, pi - 1); 
		quickSort(arr, pi + 1, high); 
	} 
} 

/* Function to print an array */
void printArray(int arr[], int size) 
{ 
	int i; 
	for (i=0; i < size; i++) 
		printf("%d ", arr[i]); 
	printf("n"); 
} 

// Driver program to test above functions 
int main() 
{
    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../../data/qsort_seq_" + std::to_string(run) + ".csv";
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
            quickSort(arr, l, r);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            //Write result to file
            outputFile << n << ", " << duration.count() << std::endl;

            //Print result to terminal
            std::cout << "qsort(" << n << "), time = " << duration.count() << "ms" << std::endl;
        }

        //Free memory
        free(arr);
    }

    // you will need to modify all parameters
    // you can try to create larger arrays by generating them with randomized elements
	int arr[] = {10, 7, 8, 9, 1, 5}; 
	int n = sizeof(arr)/sizeof(arr[0]); 
	quickSort(arr, 0, n-1); 
	printf("Sorted array: n"); 
	printArray(arr, n); 
	return 0; 
} 

