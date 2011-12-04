#ifndef GENETIC_OPERATION_H
#define GENETIC_OPERATION_H

#include "params.h"

void selection_r(param_t* params, int** parent, int* fitness, int** parentNew){}
	
void crossover(param_t* params, float pCrossover, int** parentNew, int** offspring){}

void mutation(param_t* params, float pMutation, int** offspring){}

void evaluation(param_t* params, int** offspring, int* fitness){}

void elitism(param_t* params, int elitesize, int** parent, int* fitness, int** offspring){}

#endif /* GENETIC_OPERATION_H */


