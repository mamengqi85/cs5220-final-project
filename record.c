#include <stdio.h>
#include <stdlib.h>
#include "record.h"

record_t* alloc_record(params_t* params)
{
	record_t* r = (record_t*) calloc(1, sizeof(record_t));
	record->allcost = (int**) calloc(params->popsize * params->maxGen, sizeof(int));
	record->bestcost = (int*) calloc(params->maxGen, sizeof(int));
	record->meancost = (int*) calloc(params->maxGen, sizeof(int));
	record->stdcost = (int*) calloc(params->maxGen, sizeof(int));
	record->sbest = (int*) calloc(params->len, sizeof(int));
	return r;
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


