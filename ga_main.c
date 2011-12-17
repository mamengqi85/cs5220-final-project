#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
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

void syn_entry(int* entry, int* entry_buffer, int recv_count, int rank, int size)
{
	int i;
	int* recvcounts = (int*) calloc(size, sizeof(int));
	int* displs = (int*) calloc(size, sizeof(int));
	for (i = 0; i < size; ++i) {
		recvcounts[i] = recv_count;
		displs[i] = i * recv_count;
	}
	MPI_Gatherv(entry, recvcounts[rank], MPI_FLOAT, entry_buffer, recvcounts, displs, MPI_FLOAT, size - 1, MPI_COMM_WORLD);
	free(recvcounts);
	free(displs);
}

void syn_entryf(float* entry, float* entry_buffer, int recv_count, int rank, int size)
{
	int i;
	int* recvcounts = (int*) calloc(size, sizeof(int));
	int* displs = (int*) calloc(size, sizeof(int));
	for (i = 0; i < size; ++i) {
		recvcounts[i] = recv_count;
		displs[i] = i * recv_count;
	}
	MPI_Gatherv(entry, recvcounts[rank], MPI_FLOAT, entry_buffer, recvcounts, displs, MPI_FLOAT, size - 1, MPI_COMM_WORLD);
	free(recvcounts);
	free(displs);
}

void syn(param_t* params, record_t* records, int trial, int** allcost_set, 
		int** bestcost_set, float** meancost_set, int** sbest_set, 
		int rank, int size, double* t_syn)
{
	int i, j, k;
	int* allcost_buffer = (int*) calloc(params->popsize * params->maxGen * params->deme, sizeof(int));
	int* bestcost_buffer = (int*) calloc(params->maxGen * params->deme, sizeof(int));
	float* meancost_buffer = (float*) calloc(params->maxGen * params->deme, sizeof(float));
	int* sbest_buffer = (int*) calloc(params->len * params->deme, sizeof(int));

	double t0 = MPI_Wtime();
	syn_entry(records->allcost, allcost_buffer, params->popsize * params->maxGen, rank, size);
	syn_entry(records->bestcost, bestcost_buffer, params->maxGen, rank, size);
	syn_entryf(records->meancost, meancost_buffer, params->maxGen, rank, size);
	syn_entry(records->sbest, sbest_buffer, params->len, rank, size);
	double t1 = MPI_Wtime();
	*t_syn = t1 - t0;

	for (i = 0; i < params->deme; ++i) {
		for (j = 0; j < params->popsize * params->maxGen; ++j) {
			allcost_set[trial][i * params->popsize * params->maxGen + j] = allcost_buffer[i * params->popsize * params->maxGen + j];
		}
	}

	int min = params->n_clauses;
	for (i = 0; i < params->maxGen; ++i) {
		float mean = 0.0;
		for (j = 0; j < params->deme; ++j) {
			if (bestcost_buffer[j * params->maxGen + i] < min) {
				bestcost_set[trial][i] = bestcost_buffer[j * params->maxGen + i];
				for (k = 0; k < params->len; ++k) {
					sbest_set[trial][k] = sbest_buffer[j * params->len + k];
				}
			}
			mean += meancost_buffer[j * params->maxGen + i];
		}	
		meancost_set[trial][i] = mean / params-> deme;
	}
	free(allcost_buffer);
	free(bestcost_buffer);
	free(meancost_buffer);
	free(sbest_buffer);
	/*
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
	*/
}
	
int main(int argc, char** argv)
{
	param_t* params = alloc_param();
	if (set_params(argc, argv, params) != 0) {
		exit(-1);
	}
	read_data(params);
	//print_matrix(params->clauses, params->n_clauses, params->n_sat, "clauses.txt");

	record_t* records = alloc_record(params);

	//generate initial solutions for all trials
	srand((int)time(0));
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
		allcost_set[i] = (int*) calloc(params->popsize * params->maxGen * params->deme, sizeof(int));
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

	//Initialize MPI and get rank and size
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size != params->deme) {
		if (rank == size - 1) {
			printf("please set number of threads equal to the number of subpopulation. Exit.");
		}
		exit(1);
	}

	double t_all = 0, t_comm = 0, t_syn = 0, t_comm_tmp = 0, t_syn_tmp = 0;
	//perform GA
	for (i = 0; i < params->trials; ++i) {
		double t0 = MPI_Wtime();
		ga(params, Xinitials[i], records, rank, size, &t_comm_tmp);
		syn(params, records, i, allcost_set, bestcost_set, meancost_set, sbest_set, rank, size, &t_syn_tmp);
		double t1 = MPI_Wtime();
		if (rank == size - 1) {
			t_all += t1 - t0;
			t_comm += t_comm_tmp;
			t_syn += t_syn_tmp;
		}
	}
	if (rank == size - 1) {
		t_all /= params->trials;
		t_comm /= params->trials;
		t_syn /= params->trials;
		printf("average all time: %f\n", t_all);
		printf("average comm time: %f\n", t_comm);
		printf("average syn time: %f\n", t_syn);
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

	if (rank == size - 1) {
		printf("Average best cost of last generations over all trials = %f\n", minavg);
		printf("Minimum best cost of last generations over all trials = %d\n", minf);
		printf("Number of trials where best cost is found = %d\n", minhit);

		print_matrix(&last_gen_best_costs, 1, params->trials, params->out_best_cost);
		float* avgCost = (float*) calloc(params->popsize * params->maxGen * params->deme, sizeof(float));
		for (i = 0; i < params->popsize * params-> maxGen * params->deme; ++i) {
			for (j = 0; j < params->trials; ++j) {
				avgCost[i] += allcost_set[j][i];
			}
			avgCost[i] /= params->trials;
		}
		print_matrixf(&avgCost, 1, params->popsize * params->maxGen, params->out_avg_fun_eval);

		print_matrix(&best_sol, 1, params->len, params->out_best_sol);
		print_matrixf(&fittest, 1, params->maxGen, params->out_fittest);
	}

	MPI_Finalize();
	free_record(records, params);
	free_param(params);
}
