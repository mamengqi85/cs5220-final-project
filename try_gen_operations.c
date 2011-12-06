#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "params.h"
#include "record.h"
#include "utility.h"
#include "genetic_operations.h"

int main()
{
	int testing = 2;
	srand((int)time(0));
	param_t* params = alloc_param();
	params->popsize = 6;
	params->len = 10;
	params->nCrossover = 2;
	
	int i, j;
	int** offspring = (int**) calloc(params->popsize, sizeof(int*));
	int** off = (int**) calloc(params->popsize, sizeof(int*));
	int c = 1;
	for (i = 0; i < params->popsize; ++i) {
		offspring[i] = (int*) calloc(params->len, sizeof(int));
		off[i] = (int*) calloc(params->len, sizeof(int));
		for (j = 0; j < params->len; ++j) {
			offspring[i][j] = c;
			off[i][j] = c;
			++c;
		}
	}
	int* fitness = (int*) calloc(params->popsize, sizeof(int));
	for (i = 0; i < params->popsize; ++i) {
		for (j = 0; j < params->len; ++j) {
			fitness[i] += offspring[i][j];
		}
	}
	print_population(off, fitness, params->popsize, params->len, "before.txt");

	if (testing == 0) {
		selection_r(params, off, fitness, offspring);
		print_population(offspring, fitness, params->popsize, params->len, "selection.txt");
	}

	if (testing == 1) {
		crossover(params, 0.9, offspring);
		print_population(offspring, fitness, params->popsize, params->len, "crossover.txt");
	}

	if (testing == 2) {
		mutation(params, 0.2, offspring);
		print_population(offspring, fitness, params->popsize, params->len, "mutation.txt");
	}

	//record_t* record = alloc_record(params);
}
