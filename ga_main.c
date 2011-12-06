#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ga.h"
#include "params.h"
#include "record.h"
#include "utility.h"

void read_data(param_t* params)
{
	FILE* fin;
	if ((fin = fopen(params->in_clauses, "r")) == NULL) {
		printf("error open file!\n");
		exit(1);
	}
	int i, j;
	char* p;

	char* line;
	int n_bytes = 50;
	int read_state;
	line = (char *) malloc (n_bytes + 1);

	for (i = 0; i < params->n_clauses; ++i) {
		read_state = getline (&line, (size_t* restrict) &n_bytes, fin);
		p = line;
		for (j = 0; j < params->n_sat; ++j) {
			//long a = strtol(p, &p, 10);
			params->clauses[i][j] = (int) strtol(p, &p, 10);
		}
	}
}

int main(int argc, char** argv)
{
	param_t* params = alloc_param();
	if (set_params(argc, argv, params) != 0)
		exit(-1);
	read_data(params);
	//print_matrix(params->clauses, params->n_clauses, params->n_sat, "clauses.txt");

	record_t* records = alloc_record(params);

	//generate initial solutions for all trials
	int*** Xinitials = (int***) calloc(params->trials, sizeof(int**));
	int i, j, k;
	for (i = 0; i < params->trials; ++i) {
		Xinitials[i] = (int**) calloc(params->popsize, sizeof(int*));
		for (j = 0; j < params->popsize; ++j) {
			Xinitials[i][j] = (int*) calloc(params->len, sizeof(int));
		}
	}
	
	for (i = 0; i < params->trials; ++i) {
		for (j = 0; j < params->popsize; ++j) {
			for (k = 0; k < params->len; ++k) {
				Xinitials[i][j][k] = rnd(0, 1);
			}
		}
	}
	//print_matrix(Xinitials[29], params->popsize, params->len, "matrix.txt");
	
	//initialize record variables 
	int** allcost_set = (int**) calloc(params->trials, sizeof(int*));
	for (i = 0; i < params->trials; ++i) {
		allcost_set[i] = (int*) calloc(params->popsize * params->maxGen, sizeof(int));
	}
	//print_matrix(allcost_set, params->trials, params->popsize * params->maxGen, "matrix.txt");

	int** bestcost_set = (int**) calloc(params->trials, sizeof(int*));
	for (i = 0; i < params->trials; ++i) {
		bestcost_set[i] = (int*) calloc(params->maxGen, sizeof(int));
	}

	float** meancost_set = (float**) calloc(params->trials, sizeof(float*));
	for (i = 0; i < params->trials; ++i) {
		meancost_set[i] = (float*) calloc(params->maxGen, sizeof(float));
	}

	int** sbest_set = (int**) calloc(params->trials, sizeof(int*));
	for (i = 0; i < params->trials; ++i) {
		sbest_set[i] = (int*) calloc(params->len, sizeof(int));
	}

	//perform GA
	for (i = 0; i < params->trials; ++i) {
		ga(params, Xinitials[i], records);
		for (j = 0; j < params->popsize * params->maxGen; ++j) {
			allcost_set[i][j] = records->allcost[j];
		}
		for (j = 0; j < params->maxGen; ++j) {
			bestcost_set[i][j] = records->bestcost[j];
			meancost_set[i][j] = records->meancost[j];
		}
		for (j = 0; j < params->len; ++j) {
			sbest_set[i][j] = records->sbest[j];
		}
		//print_matrix(&(records->allcost), 1, params->popsize * params->maxGen, "allcost.txt");
		//print_matrix(&(records->bestcost), 1, params->maxGen, "bestcost.txt");
		//print_matrixf(&(records->meancost), 1, params->maxGen, "meancost.txt");
		//print_matrix(&(records->sbest), 1, params->len, "sbest.txt");
	}

	//record results
	float* fittest = (float*) calloc(params->maxGen, sizeof(float));
	int minhit = 0;
	int* last_gen_best_costs = (int*) calloc(params->trials, sizeof(int));
	for (i = 0; i < params->trials; ++i)
		last_gen_best_costs[i] = bestcost_set[i][params->maxGen - 1];
	float minavg = mean(last_gen_best_costs, params->trials);
	int minf = min(last_gen_best_costs, params->trials);
	int* best_sol;

	for (i = 0; i < params->maxGen; ++i) {
		for (j = 0; j < params->trials; ++j) {
			fittest[i] += bestcost_set[j][i];
			if (i == params->maxGen - 1) {
				if (bestcost_set[j][i] == minf) {
					++minhit;
					best_sol = sbest_set[j];
				}
			}
		}
		fittest[i] /= params->trials;
	}

	printf("minavg = %f\n", minavg);
	printf("minf = %d\n", minf);
	printf("minhit = %d\n", minhit);

	print_matrix(&last_gen_best_costs, 1, params->trials, params->out_best_cost);
	float* avgCost = (float*) calloc(params->popsize * params->maxGen, sizeof(float));
	for (i = 0; i < params->popsize * params-> maxGen; ++i) {
		for (j = 0; j < params->trials; ++j) {
			avgCost[i] += allcost_set[j][i];
		}
		avgCost[i] /= params->trials;
	}
	print_matrixf(&avgCost, 1, params->popsize * params->maxGen, params->out_avg_fun_eval);

	print_matrix(&best_sol, 1, params->len, params->out_best_sol);
	print_matrixf(&fittest, 1, params->maxGen, params->out_fittest);

	free_record(records, params);
	free_param(params);
}
