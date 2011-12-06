#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "params.h"
#include "genetic_operations.h"
#include "utility.h"

#define DEBUG 0 

int fitnessSAT(param_t* params, int* individual)
{
	int f = 0;
	int i, j;
	for (i = 0; i < params->n_clauses; ++i) {
		for (j = 0; j < params->n_sat; ++j) {
			if (params->clauses[i][ j] > 0) {
				if (individual[params->clauses[i][j]] == 1) {
					++f;
					break;
				}
			} else {
				if (individual[-params->clauses[i][j]] == 0) {
					++f;
					break;
				}
			}
		}
	}
	return f;
}

void selection_r(param_t* params, int** parent, int* fitness, int** offspring)
{
	int i, j = 0;
	int base = min(fitness, params->popsize) - 1;
	int* trans = (int*) calloc(params->popsize, sizeof(int));
	for (i = 0; i < params->popsize; ++i) {
		trans[i] = fitness[i] - base;
	}
	float sum = 0.0;
	float* selection_rate = (float*) calloc(params->popsize, sizeof(float));
	for (i = 0; i < params->popsize; ++i) {
		sum += trans[i];
		selection_rate[i] = sum;
	}
	for (i = 0; i < params->popsize; ++i) {
		selection_rate[i] = selection_rate[i] / sum;
	}

	int firstInd = 0;
	for (i = 0; i < params->popsize / 2; ++i) {
		float r = rndf();

		if (DEBUG) {
			printf("======1st random:%f\t======\n", r);
		}
		for (j = 0; j < params->popsize; ++j) {
			if (r < selection_rate[j]) {
				memcpy(offspring[2 * i], parent[j], params->len * sizeof(int));
				firstInd = j;
				if (DEBUG) {
					printf("fisrt ind = %d\n", j);
				}

				break;
			}
		}
		int isFound = 0;
		while (isFound == 0) {
			r = rndf();
			if (DEBUG) {
				printf("2nd random:%f\n", r);
			}
			for (j = 0; j < params->popsize; ++j) {
				if (r < selection_rate[j]) {
					if (DEBUG) {
						printf("same j:%d\n", j);
					}
					if (j != firstInd) {
						memcpy(offspring[2 * i + 1], parent[j], params->len * sizeof(int));
						if (DEBUG) {
							printf("final 2nd ind = %d\n", j);
						}
						isFound = 1;
						break;
					}
					break;
				}
			}
		}
	}
}

void crossover(param_t* params, float pCrossover, int** offspring)
{
	int i, j, k;
	int* shuffle;
	int nc = params->nCrossover;
	if (params->nCrossover % 2 == 0) {
		shuffle = (int*) calloc(nc, sizeof(int));
	} else {
		++nc;
		shuffle = (int*) calloc(nc, sizeof(int));
	}

	for (i = 0; i < params->popsize / 2; ++i) {
		float r = rndf();
		if (r > pCrossover) {
			if (DEBUG) {
				printf("r = %f\ti = %d\tpC = %f\n", r, i, pCrossover);
			}
			continue;
		}
		int* shuffle_tmp = randperm(params->len - 1);
		for (j = 0; j < params->nCrossover; ++j) {
			shuffle[j] = shuffle_tmp[j] + 1;
		}
		if (params->nCrossover % 2 == 1) {
			shuffle[params->nCrossover] = params->len;
		}
		sort(shuffle, nc);
		
		if (DEBUG) {
			printf("shuffle\n");
			for (j = 0; j < nc; ++j) {
				printf("%d\t", shuffle[j]);
			}
			printf("\n");
		}

		int* offspring1 = (int*) calloc(params->len, sizeof(int));
		int* offspring2 = (int*) calloc(params->len, sizeof(int));
		memcpy(offspring1, offspring[2 * i], params->len * sizeof(int));
		memcpy(offspring2, offspring[2 * i + 1], params->len * sizeof(int));

		for (j = 0; j < nc / 2; ++j) {
			int pos_s = shuffle[2 * j];
			int pos_e = shuffle[2 * j + 1];
			for (k = pos_s; k < pos_e; ++k) {
				offspring1[k] = offspring[2 * i + 1][k];
				offspring2[k] = offspring[2 * i][k];
			}
		}
		memcpy(offspring[2 * i], offspring1, params->len * sizeof(int));
		memcpy(offspring[2 * i + 1], offspring2, params->len * sizeof(int));
	}
}

void mutation(param_t* params, float pMutation, int** offspring)
{
	int i, j;
	for (i = 0; i < params->popsize; ++i) {
		if (DEBUG) {
			printf("%dth:,", i);
		}
		for (j = 0; j < params->len; ++j) {
			float r = rndf();
			if (DEBUG) {
				printf("%f,", r);
			}
			if (r < pMutation) {
				offspring[i][j] = !(offspring[i][j]);
			}
		}
		if (DEBUG) {
			printf("\n");
		}
	}
}
		
void evaluation(param_t* params, int** offspring, int* fitness)
{
	int i;
	for (i = 0; i < params->popsize; ++i) {
		fitness[i] = fitnessSAT(params, offspring[i]);
	}
}

void elitism(param_t* params, int elitesize, int** parent, int* fitness, int* p_fitness, int** offspring)
{
	int i;
	for (i = 0; i < elitesize; ++i) {
		if (DEBUG)
		{
			printf("elitesize = %d, son_ind = %d, parent_ind = %d\n", elitesize, i, params->popsize-i-1);
		}
		memcpy(offspring[i], parent[params->popsize - i - 1], params->len * sizeof(int));
		fitness[i] = p_fitness[params->popsize - i - 1];
	}
}
