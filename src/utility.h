#ifndef CS4204_P2_DC_UTILITY_H
#define CS4204_P2_DC_UTILITY_H

/* UTILITY FUNCTIONS */
#include <cstdio>

/* Function to print an array */
void printArray(int A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

#endif //CS4204_P2_DC_UTILITY_H
