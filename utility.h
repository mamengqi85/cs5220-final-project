#ifndef UTILITY_H
#define UTILITY_H

#include "params.h"

//generate an integer from [a, b]
int rnd(int a, int b);


//given n, return a permutation of 0:n-1 in random order
int* randperm(int n);

//reorder matrix offspring by row, with the given order
void reorder(param_t* params, int** offspring, int* order);

int max(int* array, int size);
int min(int* array, int size);
float mean(int* array, int size);

//sort the array by ascending order. return the original index after sorted.
int* sort(int* array, int size);

#endif /* UTILITY */
