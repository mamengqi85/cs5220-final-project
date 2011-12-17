#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ga.h"
#include "genetic_operations.h"
#include "params.h"
#include "record.h"
#include "time.h"
#include "utility.h"

void ga(param_t* params, int** Xinitial, record_t* records, double* t_eval, double* t_cm)
{
	int i, j;
	clock_t t0, t1;
	int* fitness = (int*) calloc(params->popsize, sizeof(int));
	int* p_fitness = (int*) calloc(params->popsize, sizeof(int));
	int** parent = (int**) calloc(params->popsize, sizeof(int*));
	for (i = 0; i < params->popsize; ++i) {
		parent[i] = (int*) calloc(params->len, sizeof(int));
		for (j = 0; j < params->len; ++j) {
			parent[i][j] = Xinitial[i][j];
		}
	}
	t0 = clock();
	evaluation(params, parent, fitness);
	t1 = clock();
	*t_eval = (double) (t1 - t0) / CLOCKS_PER_SEC;
	*t_cm = 0;
	int* order = sort(fitness, params->popsize);
	reorder(params, parent, order);
	memcpy(p_fitness, fitness, params->popsize * sizeof(int));
	int max_fitness = 0;
	srand((int)time(0));
	float dc = (params->pCrossover_s - params->pCrossover_e) / params->maxGen;
	float dm = (params->pMutation_s - params->pMutation_e) / params->maxGen;
	float de = (params->elitesize_e - params->elitesize_s) / (float) params->maxGen;

	int** offspring = (int**) calloc(params->popsize, sizeof(int*));
	for (i = 0; i < params->popsize; ++i) {
		offspring[i] = (int*) calloc(params->len, sizeof(int));
	}

	for (i = 0; i < params->maxGen; ++i) {
		float pCrossover = params->pCrossover_s - dc * i;
		float pMutation = params->pMutation_s - dm * i;
		int elitesize = (int)(params->elitesize_s + de * i);

		selection_r(params, parent, fitness, offspring);
		t0 = clock();
		crossover(params, pCrossover, offspring);
		mutation(params, pMutation, offspring);
		t1 = clock();
		*t_cm = *t_cm + (double)(t1 - t0) / CLOCKS_PER_SEC;

		t0 = clock();
		evaluation(params, offspring, fitness);
		t1 = clock();
		*t_eval = *t_eval + (double) (t1 - t0) / CLOCKS_PER_SEC;

		int* order = sort(fitness, params->popsize);
		reorder(params, offspring, order);

		//print_population(offspring, fitness, params->popsize, params->len, "population.txt");
		elitism(params, elitesize, parent, fitness, p_fitness, offspring);
		
		order = sort(fitness, params->popsize);
		reorder(params, offspring, order);
		recorder(params, i, &max_fitness, offspring, fitness, records);

		for (j = 0; j < params->popsize; ++j) {
			memcpy(parent[j], offspring[j], params->len * sizeof(int));
		}
		memcpy(p_fitness, fitness, params->popsize * sizeof(int));
		//print_population(parent, p_fitness, params->popsize, params->len, "father.txt");
		//print_population(offspring, fitness, params->popsize, params->len, "offspring.txt");
	}
}
