#include <stdio.h>
#include <stdlib.h>
#include "params.h"
#include "record.h"
#include "utility.h"

record_t* alloc_record(param_t* params)
{
	record_t* records = (record_t*) calloc(1, sizeof(record_t));

	records->allcost = (int*) calloc(params->popsize * params->maxGen, sizeof(int));
	records->bestcost = (int*) calloc(params->maxGen, sizeof(int));
	records->meancost = (float*) calloc(params->maxGen, sizeof(float));
	records->sbest = (int*) calloc(params->len, sizeof(int));
	return records;
}

void free_record(record_t* records, param_t* params)
{
	free(records->allcost);
	free(records->bestcost);
	free(records->meancost);
	free(records->sbest);
	free(records);
}

void recorder(param_t* params, int gen, int* max_fitness, int** offspring, int* fitness, record_t* records)
{
	int MAXF = params->n_clauses;
	int i;
	for (i = 0; i < params->popsize; ++i) {
		records->allcost[gen * params->popsize + i] = MAXF - fitness[i];
	}
	records->bestcost[gen] = MAXF - fitness[params->popsize - 1];
	records->meancost[gen] = MAXF - mean(fitness, params->popsize);
	if (fitness[params->popsize - 1] > *max_fitness) {
		for (i = 0; i < params->len; ++i) {
			records->sbest[i] = offspring[params->popsize - 1][i];
		}
		*max_fitness = fitness[params->popsize - 1];
	}
}
