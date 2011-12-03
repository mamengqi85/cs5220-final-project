#include <stdio.h>
#include <stdlib.h>
#include "record.h"

record_t* alloc_record(params_t* params)
{
	record_t* record = (record_t*) calloc(1, sizeof(record_t));

	record->allcost = (int**) calloc(params->maxGen, sizeof(int*));
	int i;
	for (i= 0; i < params->maxGen; ++i) {
		record->allcost[i] = (int*) calloc(params->popsize, sizeof(int));
	}

	record->bestcost = (int*) calloc(params->maxGen, sizeof(int));
	record->meancost = (int*) calloc(params->maxGen, sizeof(int));
	record->sbest = (int*) calloc(params->len, sizeof(int));
	return record;
}

void free_record(record_t* r)
{
	free(record->allcost);
	free(record->bestcost);
	free(record->meancost);
	free(record->stdcost);
	free(record->sbest)

void recorder(params_t* params, int* max_fitness, int** offspring, record_t* records)
{

}


