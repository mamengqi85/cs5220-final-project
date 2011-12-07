#ifndef UTILITY_H
#define UTILITY_H

#include "params.h"

//generate an integer from [a, b]
int rnd(int a, int b);

//generate a float from [0, 1]
float rndf();

//given n, return a permutation of 0:n-1 in random order
int* randperm(int n);

//reorder matrix offspring by row, with the given order
void reorder(param_t* params, int** offspring, int* order);

int max(int* array, int size);
int min(int* array, int size);
float mean(int* array, int size);

//sort the array by ascending order. return the original index after sorted.
int* sort(int* array, int size);

//transfer between 2D and 1D
int* de_d(int** matrix, int row, int col);
int** in_d(int* matrix, int rwo, int col);

//output the matrix to a specific file
void print_matrix(int** matrix, int row, int col, char* fname);
void print_matrixf(float** matrix, int row, int col, char* fname);
void print_1d(int* matrix, int row, int col, char* fname);

//output the population, fitness, and cumulated selection rate to a specific file
void print_population(int** population, int* fitness, int row, int col, char* fname);
#endif /* UTILITY */
