#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

/*
int round(float r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}
*/

//generate an integer from [a, b]
int rnd(int a, int b)
{
	int ans = 0;
	//ans=(int)(rand() * (b - a + 1) / (1.0 * RAND_MAX) + a);
	ans = rand() % (b - a + 1) + a;
	return ans;
}

//given n, return a permutation of 0:n-1 in random order
int* randperm(int n)
{
	int* perm = (int*) calloc(n, sizeof(int));
	int i, j, tmp;
	for (i = 0; i < n; ++i) {
		perm[i] = i;
	}
	for (i = 0; i < n; ++i) {
		j = rand() % (n - i) + i;
		tmp = perm[j];
		perm[j] = perm[i];
		perm[i] = tmp;
	}
	return perm;
}

void reorder(param_t* params, int** offspring, int* order)
{
	int** tmp = (int**) calloc(params->popsize, sizeof(int*));
	int i, j;
	for (i = 0; i < params->popsize; ++i) {
		tmp[i] = (int*) calloc(params->len, sizeof(int)); 
		for (j = 0; j < params->len; ++j) {
			tmp[i][j] = offspring[order[i]][j];
		}
	}
	memcpy(offspring, tmp, params->popsize * params->len * sizeof(int));
	free(tmp);
}

int max(int* array, int size)
{
	int i;
	int maxi = INT_MIN;
	for (i = 0; i < size; ++i) {
	   if (array[i] > maxi) {
		   maxi = array[i];
	   }
	}
	return maxi;
}

int min(int* array, int size)
{
	int i;
	int mini = INT_MAX;
	for (i = 0; i < size; ++i) {
	   if (array[i] < mini) {
		   mini = array[i];
	   }
	}
	return mini;
}

float mean(int* array, int size)
{
	if (size == 0) return 0;
	int i;
	float sum = 0.0;
	for (i = 0; i < size; ++i) {
		sum += array[i];
	}
	sum /= size;
	return sum;
}

void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(int* A, int p, int r, int* order)
{
	int x = A[r];
	int i = p - 1;
	int j;
	for (j = p; j < r; ++j) {
		if (A[j] <= x) {
			++i;
			swap(A + i, A + j);
			swap(order + i, order + j);
		}
	}
	swap(A + i + 1, A + r);
	swap(order + i + 1, order + r);
	return i + 1;
}

void quicksort(int* A, int p, int r, int* order)
{	
	if (p < r) {
		int q = partition(A, p, r, order);
		quicksort(A, p, q - 1, order);
		quicksort(A, q + 1, r, order);
	}
}

//quick sort
int* sort(int* array, int size)
{
	int* order = (int*) calloc(size, sizeof(int));
	int i;
	for (i = 0; i < size; ++i)
		order[i] = i;
	quicksort(array, 0, size - 1, order);
	return order;
}
