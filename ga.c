#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ga.h"
#include "params.h"
#include "record.h"
#include "utility.h"

void ga(param_t* params, int** Xinitials, record_t* records)
{
	row = params->popsize;
	col = params->len;

	int* fitness = (int*) calloc(row, sizeof(int));
	int** parent = Xinitial;
	evaluation(params, parent, fitness);
	int max_fitness = 0;
	record_t* records = alloc_record(params);
	srand((int)time(0));
	float dc = (params->pCrossover_s - params->pCrossover_e) / params->maxGen;
	float dm = (params->pMutation_s - params->pMutation_e) / params->maxGen;
	float de = (params->elitesize_e - params->elitesize_s) / params->maxGen;

	int i;
	for (i = 0; i < params->maxGen; ++i) {
		float pCrossover = pCrossover_s - dc * i;
		float pMutation = pMutation_s - dm * i;
		float elitesize = pCrossover_s - dc * i;
		int** parentNew;
		int** offspring;

		selection_r(params, parent, fitness, parentNew);
		crossover(params, parentNew, offspring);
		mutation(params, offspring);
		evaluation(params, offspring, fitness);
		elitism(params, parent, fitness, offspring);
		recorder(params, &max_fitness, offspring, records);
		randomizer(params, offspring);

		parent = offspring;
	}
}
