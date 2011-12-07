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

int* evaluation_g(param_t* params, int* offspring, int rank, int size)
{
	int row;
	if (params->popsize % size != 0) {
		row = params->popsize / size + 1;
		if (rank == size - 1) {
			row = params->popsize - row * (size - 1);
		}
	} else {
		row = params->popsize / size;
	}
	int* fitness = (int*) calloc(row, sizeof(int));
	int i;
	for (i = 0; i < row; ++i) {
		fitness[i] = fitnessSAT(params, offspring + i * params->len);
	}
	free(offspring);
	return fitness;
}

int* dis(param_t* params, int** offspring, int rank, int size, int real_size)
{
	int i; 
	int row, last_row;

	if (params->popsize % size != 0) {
		row = params->popsize / size + 1;
		last_row = params->popsize - row * (size - 1);
	} else {
		row = params->popsize / size;
		last_row = row;
	}

	int* offspring_1d = de_d(offspring, params->popsize, params->len);

	int* recvcounts = (int*) calloc(real_size, sizeof(int));
	int* displs = (int*) calloc(real_size, sizeof(int));
	
	int* sub_offspring = (int*) calloc(row * params->len, sizeof(int*));
	for (i = 0; i < real_size; ++i) {
		recvcounts[i] = row * params->len;
		displs[i] = i * row * params->len;
		if (i == size - 1) {
			recvcounts[i] = last_row * params->len;
		}
		if (i > size - 1) {
			recvcounts[i] = 0;
			displs[i] = 0;
		}

		//printf("rank = %d, i = %d, recvcount = %d, displs = %d, rcvRank = %d, row = %d, last_row = %d, root = %d, realsize = %d\n", rank, i,recvcounts[i], displs[i], recvcounts[rank], row, last_row, size - 1, real_size);
	}
	//MPI_Scatterv(offspring + displs[rank], params->len, displs, MPI_INT, sub_offspring[i], recvcount, MPI_INT, size - 1, MPI_COMM_WORLD)
	MPI_Scatterv(offspring_1d, recvcounts, displs, MPI_INT, sub_offspring, recvcounts[rank], MPI_INT, size - 1, MPI_COMM_WORLD);
	//MPI_Scatter(SendAddress, SendCount, disp, SendDatatype, 
	//		RecvAddress, RecvCount, RecvDatatype, size - 1, MPI_COMM_WORLD)
	//MPI_Alltoall(SendAddress, SendCount, SendDatatype,
	//		RecvAddress, RecvCount, RecvDatatype, Comm)
	free(offspring_1d);
	return sub_offspring;
}

void syn(param_t* params, int* fitness, int* sub_fitness, int rank, int size, int real_size)
{
	int i;
	int row, last_row;

	if (params->popsize % size != 0) {
		row = params->popsize / size + 1;
		last_row = params->popsize - row * (size - 1);
	} else {
		row = params->popsize / size;
		last_row = row;
	}

	int* recvcounts = (int*) calloc(real_size, sizeof(int));
	int* displs = (int*) calloc(real_size, sizeof(int));
	for (i = 0; i < real_size; ++i) {
		displs[i] = i * row;
		if (i < size - 1) {
			recvcounts[i] = row;
		} else {
			if (i == size - 1) {
				recvcounts[i] = last_row;
			} else {
				recvcounts[i] = 0;
				displs[i] = 0;
			}
		}
	//	printf("i = %d, recvcount = %d, displs = %d\n", i,recvcounts[i], displs[i]);
	}
//printf("lastrow = %d, recvcount = %d, displ = %d, root = %d\n", last_row, recvcounts[0], displs[0], size - 1);
	MPI_Gatherv(sub_fitness, recvcounts[rank], MPI_INT, fitness, recvcounts, displs, MPI_INT, size - 1, MPI_COMM_WORLD);
	//MPI_Gather(SendAddress, SendCount, SendDatatype, RecvAddress, RecvCount, displs, RecvDatatype, Root, Comm)
	//MPI_Alltoall(SendAddress, SendCount, SendDatatype,
	//		RecvAddress, RecvCount, RecvDatatype, Comm)
	free(sub_fitness);
}

void ga(param_t* params, int** Xinitial, record_t* records, int rank, int size, int real_size)
{
	int i, j;
	int* fitness = (int*) calloc(params->popsize, sizeof(int));
	int* p_fitness = (int*) calloc(params->popsize, sizeof(int));
	int** parent = (int**) calloc(params->popsize, sizeof(int*));
	for (i = 0; i < params->popsize; ++i) {
		parent[i] = (int*) calloc(params->len, sizeof(int));
		for (j = 0; j < params->len; ++j) {
			parent[i][j] = Xinitial[i][j];
		}
	}

	int* sub_offspring = dis(params, parent, rank, size, real_size);
	int* sub_fitness = evaluation_g(params, sub_offspring, rank, size);
	//MPI_Barrier(MPI_COMM_WORLD);
	syn(params, fitness, sub_fitness, rank, size, real_size);

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

	for (i = 0; i < params->maxGen; ++i) {
		double t0_ga = MPI_Wtime();

		float pCrossover = params->pCrossover_s - dc * i;
		float pMutation = params->pMutation_s - dm * i;
		int elitesize = (int)(params->elitesize_s + de * i);

		if (rank == size - 1) {
   	 		selection_r(params, parent, fitness, offspring);
   	 		crossover(params, pCrossover, offspring);
   	 		mutation(params, pMutation, offspring);
		}

		double t0 = MPI_Wtime();
		sub_offspring = dis(params, offspring, rank, size, real_size);
		sub_fitness = evaluation_g(params, sub_offspring, rank, size);
		//MPI_Barrier(MPI_COMM_WORLD);
		syn(params, fitness, sub_fitness, rank, size, real_size);
		double t1 = MPI_Wtime();
if(rank == size -1)
		printf("eval time: %f\n", t1 - t0);

		if (rank == size - 1) {
			int* order = sort(fitness, params->popsize);
			reorder(params, offspring, order);

			elitism(params, elitesize, parent, fitness, p_fitness, offspring);
			
			order = sort(fitness, params->popsize);
			reorder(params, offspring, order);
			recorder(params, i, &max_fitness, offspring, fitness, records);

			for (j = 0; j < params->popsize; ++j) {
				memcpy(parent[j], offspring[j], params->len * sizeof(int));
			}
			memcpy(p_fitness, fitness, params->popsize * sizeof(int));
		}
//print_population(offspring, fitness, params->popsize, params->len, "end_population.txt");
//print_population(parent, p_fitness, params->popsize, params->len, "end_father.txt");
double t1_ga = MPI_Wtime();
if(rank == size - 1)
printf("ga alg time %f\n", t1_ga - t0_ga);
	}
}
