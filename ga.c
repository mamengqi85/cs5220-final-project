#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ga.h"
#include "genetic_operations.h"
#include "params.h"
#include "record.h"
#include "utility.h"

void ga(param_t* params, int** Xinitial, record_t* records)
{
	int row = params->popsize;

	int* fitness = (int*) calloc(row, sizeof(int));
	int** parent = Xinitial;
	evaluation(params, parent, fitness);
	int max_fitness = 0;
	records = alloc_record(params);
	srand((int)time(0));
	float dc = (params->pCrossover_s - params->pCrossover_e) / params->maxGen;
	float dm = (params->pMutation_s - params->pMutation_e) / params->maxGen;
	float de = (params->elitesize_e - params->elitesize_s) / params->maxGen;

	int i;
	for (i = 0; i < params->maxGen; ++i) {
		float pCrossover = params->pCrossover_s - dc * i;
		float pMutation = params->pMutation_s - dm * i;
		float elitesize = params->pCrossover_s - de * i;
		int** parentNew;
		int** offspring;

		selection_r(params, parent, fitness, parentNew);
		crossover(params, pCrossover, parentNew, offspring);
		mutation(params, pMutation, offspring);
		evaluation(params, offspring, fitness);
		elitism(params, elitesize, parent, fitness, offspring);
		recorder(params, &max_fitness, offspring, records);
		int* order = sort(fitness, params->popsize);
		reorder(params, offspring, order);

		parent = offspring;
	}
}
