#include <stdio.h>
#include <stdlib.h>
#include "params.h"
#include "record.h"

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

void recorder(param_t* params, int* max_fitness, int** offspring, record_t* records)
{

}

