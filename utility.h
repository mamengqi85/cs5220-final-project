#ifndef UTILITY_H
#define UTILITY_H

#include "params.h"

//int round(float r);
int rnd(int a, int b);
int* randperm(int n);
void reorder(param_t* params, int** offspring, int* order);
int max(int* array, int size);
int min(int* array, int size);
float mean(int* array, int size);
int* sort(int* array, int size);

#endif /* UTILITY */
