/* C program for Merge Sort */
#include<stdlib.h> 
#include<stdio.h>
#include "../main.h"
#include "../../../../../../../usr/include/c++/7/string"
#include "../../../../../../../usr/include/c++/7/fstream"
#include "../../../../../../../usr/include/c++/7/chrono"
#include "../../../../../../../usr/include/c++/7/iostream"

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int arr[], int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 = r - m; 

	/* create temp arrays */
	int L[n1], R[n2]; 

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1+ j]; 

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
} 

/* l is for left index and r is right index of the 
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r) 
{ 
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		int m = l+(r-l)/2; 

		// Sort first and second halves 
		mergeSort(arr, l, m); 
		mergeSort(arr, m+1, r); 

		merge(arr, l, m, r); 
	} 
} 

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size) 
{ 
	int i; 
	for (i=0; i < size; i++) 
		printf("%d ", A[i]); 
	printf("\n"); 
} 

/* Driver program to test above functions */
int main() 
{
    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../../data/merge_seq_" + std::to_string(run) + ".csv";
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
            mergeSort(arr, l, r);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            //Write result to file
            outputFile << n << ", " << duration.count() << std::endl;

            //Print result to terminal
            std::cout << "mergesort(" << n << "), time = " << duration.count() << "ms" << std::endl;
        }

        //Free memory
        free(arr);
    }
} 

