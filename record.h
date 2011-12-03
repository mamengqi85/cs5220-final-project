#ifndef RECORD_H
#define RECORD_H

typedef struct record_t{
	int* allcost;		/* cost of the whole population			*/
	int* bestcost;		/* best cost in the population			*/
	int* meancost;		/* mean of the cost of the population	*/
	int* stdcost;		/* std of the cost of the population	*/
	int* sbest;			/* current best solution				*/
} record_t;

record_t* alloc_record(params_t* params);
free_record(record_t* r);
void recorder(param_t* params, int* max_fitness, int** offspring, record_t* records);

#endif /* RECORD_H */
