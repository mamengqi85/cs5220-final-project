#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include "ga.h"
#include "genetic_operations.h"
#include "params.h"
#include "record.h"
#include "utility.h"

void print_population_g(int** population, int* fitness, int row, int col, char* fname, int rank)
{
	char str_rank[10];
	sprintf(str_rank, "%d", rank);
	fname = strcat(str_rank, fname);
	FILE* fp = fopen(fname, "w");
	int i, j;

	int base = min(fitness, row) - 1;
	int* trans = (int*) calloc(row, sizeof(int));
	for (i = 0; i < row; ++i) {
		trans[i] = fitness[i] - base;
	}
	float sum = 0.0;
	float* selection_rate = (float*) calloc(row, sizeof(float));
	for (i = 0; i < row; ++i) {
		sum += trans[i];
		selection_rate[i] = sum;
	}
	for (i = 0; i < row; ++i) {
		selection_rate[i] = selection_rate[i] / sum;
	}

	for (i = 0; i < row; ++i) {
		for (j = 0; j < col; ++j) {
			fprintf(fp, "%d\t", population[i][j]);
		}
		fprintf(fp, "%d\t%f\n", fitness[i], selection_rate[i]);
	}
	fclose(fp);
}

void ga(param_t* params, int** Xinitial, record_t* records, int rank, int size)
{
	int i, j, k;
	int counter = 0;
	int* fitness = (int*) calloc(params->popsize, sizeof(int));
	int* p_fitness = (int*) calloc(params->popsize, sizeof(int));
	int** parent = (int**) calloc(params->popsize, sizeof(int*));
	for (i = 0; i < params->popsize; ++i) {
		parent[i] = (int*) calloc(params->len, sizeof(int));
		for (j = 0; j < params->len; ++j) {
			parent[i][j] = Xinitial[i][j];
		}
	}
	evaluation(params, parent, fitness);
	int* order = sort(fitness, params->popsize);
	reorder(params, parent, order);
	memcpy(p_fitness, fitness, params->popsize * sizeof(int));
	int max_fitness = 0;
	srand((int)time(0));
	float dc = (params->pCrossover_s - params->pCrossover_e) / params->maxGen;
	float dm = (params->pMutation_s - params->pMutation_e) / params->maxGen;
	float de = (params->elitesize_e - params->elitesize_s) / (float) params->maxGen;

	int** offspring = (int**) calloc(params->popsize, sizeof(int*));
	for (i = 0; i < params->popsize; ++i) {
		offspring[i] = (int*) calloc(params->len, sizeof(int));
	}

	//initialize communication
	int sender, receiver;
	//int is_fisrt = 1;
	int* offspring_i = (int*) calloc(params->alien * params->len, sizeof(int));
	int* offspring_o = (int*) calloc(params->alien * params->len, sizeof(int));
	int* fitness_i = (int*) calloc(params->alien, sizeof(int));
	int* fitness_o = (int*) calloc(params->alien, sizeof(int));

	//MPI_Request send_req_o, send_req_f, recv_req_o, recv_req_f;
	//MPI_Status send_status_o, send_status_f, recv_status_o, recv_status_f;

	if (rank == 0) {
		sender = size - 1;
	} else {
		sender = rank - 1;
	}
	if (rank == size - 1) {
		receiver = 0;
	} else {
		receiver = rank + 1;
	}


	//ga loop
	for (i = 0; i < params->maxGen; ++i) {
		float pCrossover = params->pCrossover_s - dc * i;
		float pMutation = params->pMutation_s - dm * i;
		int elitesize = (int)(params->elitesize_s + de * i);

		selection_r(params, parent, fitness, offspring);
		crossover(params, pCrossover, offspring);
		mutation(params, pMutation, offspring);
		evaluation(params, offspring, fitness);

		int* order = sort(fitness, params->popsize);
		reorder(params, offspring, order);

		//print_population(offspring, fitness, params->popsize, params->len, "population.txt");
		elitism(params, elitesize, parent, fitness, p_fitness, offspring);
		
		order = sort(fitness, params->popsize);
		reorder(params, offspring, order);

		++counter;
		//communicate
		if (counter == params->freq) {
/*
			if (!is_first) {
				MPI_Wait(send_req_o, send_status_o);
				MPI_Wait(send_req_f, send_status_f);
				MPI_Wait(recv_req_o, recv_status_o);
				MPI_Wait(recv_req_f, recv_status_f);
			} else {
				is_first = 0;
			}
*/	
			for (j = 0; j < params->alien; ++j) {
				for (k = 0; k < params->len; ++k) {
					offspring_o[j * params->len + k] = offspring[params->popsize - 1 - j][k];
				}
				fitness_o[j] = fitness[params->popsize - 1 - j];
			}
/*
			MPI_Isend(offspring_o, params->alien * params->len, MPI_INT,
					receiver, 0, MPI_COMM_WORLD, send_req_o);
			MPI_Isend(fitness_o, params->alien, MPI_INT,
					receiver, 1, MPI_COMM_WORLD, send_req_f);
			MPI_Irecv(offspring_i, params->alien * params->len, MPI_INT,
					sender, 0, MPI_COMM_WORLD, recv_req_o);
			MPI_Irecv(fitness_i, params->alien, MPI_INT,
					sender, 0, MPI_COMM_WORLD, recv_req_f);
*/
/*
			MPI_Rsend(offspring_o, params->alien * params->len, MPI_INT,
					receiver, 0, MPI_COMM_WORLD);
			MPI_Rsend(fitness_o, params->alien, MPI_INT,
					receiver, 1, MPI_COMM_WORLD);
			MPI_Recv(offspring_i, params->alien * params->len, MPI_INT,
					sender, 0, MPI_COMM_WORLD, NULL);
			MPI_Recv(fitness_i, params->alien, MPI_INT,
					sender, 1, MPI_COMM_WORLD, NULL);
*/
			MPI_Sendrecv(offspring_o, params->alien * params->len, MPI_INT, receiver, 0, offspring_i, params->alien * params->len, MPI_INT, sender, 0, MPI_COMM_WORLD, NULL);
			MPI_Sendrecv(fitness_o, params->alien, MPI_INT, receiver, 0, fitness_i, params->alien, MPI_INT, sender, 0, MPI_COMM_WORLD, NULL);
			for (j = 0; j < params->alien; ++j) {
				for (k = 0; k < params->len; ++k) {
					offspring[params->popsize - 1 - j][k] =	offspring_o[j * params->len + k];
				}
				fitness[params->popsize - 1 - k] = fitness_o[j];
			}

			counter = 0;
		}

		order = sort(fitness, params->popsize);
		reorder(params, offspring, order);
		recorder(params, i, &max_fitness, offspring, fitness, records);

		for (j = 0; j < params->popsize; ++j) {
			memcpy(parent[j], offspring[j], params->len * sizeof(int));
		}
		memcpy(p_fitness, fitness, params->popsize * sizeof(int));
		//print_population(parent, p_fitness, params->popsize, params->len, "father.txt");
		//print_population(offspring, fitness, params->popsize, params->len, "offspring.txt");
	}
}
