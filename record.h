#ifndef RECORD_H
#define RECORD_H

#include "params.h"

typedef struct record_t{
	int* allcost;		/* cost of the whole population			*/
	int* bestcost;		/* best cost in the population			*/
	float* meancost;		/* mean of the cost of the population	*/
	int* sbest;			/* current best solution				*/
} record_t;

record_t* alloc_record(param_t* params);
void free_record(record_t* records, param_t* params);
void recorder(param_t* params, int gen, int* max_fitness, int** offspring, int* fitness, record_t* records);

#endif /* RECORD_H */
