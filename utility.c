#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

int rnd(int a, int b)
{
	int ans = 0;
	ans = rand() % (b - a + 1) + a;
	return ans;
}

float rndf()
{
	return (float)rand()/RAND_MAX;
}

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
	for (i = 0; i < params->popsize; ++i) {
		memcpy(offspring[i], tmp[i], params->len * sizeof(int));
	}
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

int* de_d(int** matrix, int row, int col)
{
	int i, j;
	int* new_mat = (int*) calloc(row * col, sizeof(int));
	for (i = 0; i < row; ++i) {
		for (j = 0; j < col; ++j) {
			new_mat[i * col + j] = matrix[i][j];
		}
	}

	return new_mat;
}

int** in_d(int* matrix, int row, int col)
{
	int i, j;
	int** new_mat = (int**) calloc(row, sizeof(int*));
	for (i = 0; i < row; ++i) {
		new_mat[i] = (int*) calloc(col, sizeof(int));
		for (j = 0; j < col; ++j) {
			new_mat[i][j] = matrix[i * col + j];
		}
	}
	return new_mat;
}

void print_matrix(int** matrix, int row, int col, char* fname)
{
	FILE* fp = fopen(fname, "w");
	int i, j;
	for (i = 0; i < row; ++i) {
		for (j = 0; j < col; ++j) {
			fprintf(fp, "%d\t", matrix[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void print_matrixf(float** matrix, int row, int col, char* fname)
{
	FILE* fp = fopen(fname, "w");
	int i, j;
	for (i = 0; i < row; ++i) {
		for (j = 0; j < col; ++j) {
			fprintf(fp, "%f\t", matrix[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void print_1d(int* matrix, int row, int col, char* fname)
{
	int** new_mat = in_d(matrix, row, col);
	print_matrix(new_mat, row, col, fname);
}

void print_population(int** population, int* fitness, int row, int col, char* fname)
{
	FILE* fp = fopen(fname, "w");
	int i, j;

	int base = min(fitness, row) - 1;
	int* trans = (int*) calloc(row, sizeof(int));
	for (i = 0; i < row; ++i) {
		trans[i] = fitness[i] - base;
	}
	float sum = 0.0;
	float* selection_rate = (float*) calloc(row, sizeof(float));
	for (i = 0; i < row; ++i) {
		sum += trans[i];
		selection_rate[i] = sum;
	}
	for (i = 0; i < row; ++i) {
		selection_rate[i] = selection_rate[i] / sum;
	}

	for (i = 0; i < row; ++i) {
		for (j = 0; j < col; ++j) {
			fprintf(fp, "%d\t", population[i][j]);
		}
		fprintf(fp, "%d\t%f\n", fitness[i], selection_rate[i]);
	}
	fclose(fp);
}
