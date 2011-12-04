#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "params.h"

param_t* alloc_param()
{
	int fname_len = 100;
	param_t* params = (param_t*) calloc(1, sizeof(param_t));
	params->n_sat = 3;
	params->n_clauses = 860;
	params->clauses = (int**) calloc(params->n_clauses, sizeof(int*));
	int i;
	for (i = 0; i < params->n_clauses; ++i) {
		(params->clauses)[i] = (int*) calloc(params->n_sat, sizeof(int));
	}
	params->in_clauses = (char*) calloc(fname_len, sizeof(char));
	params->out_best_cost = (char*) calloc(fname_len, sizeof(char));
	params->out_avg_fun_eval = (char*) calloc(fname_len, sizeof(char));
	params->out_min_hit = (char*) calloc(fname_len, sizeof(char));
	params->out_cpu_time = (char*) calloc(fname_len, sizeof(char));
	params->out_best_sol = (char*) calloc(fname_len, sizeof(char));
	params->out_fittest = (char*) calloc(fname_len, sizeof(char));

	return params;
}

void free_param(param_t* params)
{
	int i;
	for (i = 0; i < params->n_clauses; ++i) {
		free(params->clauses[i]);
	}
	free(params->in_clauses);
	free(params->out_best_cost);
	free(params->out_avg_fun_eval);
	free(params->out_min_hit);
	free(params->out_cpu_time);
	free(params->out_best_sol);
	free(params->out_fittest);
	free(params);
}

static void default_params(param_t* params)
{
	params->trials		= 30;
	params->len			= 200;
	params->popsize		= 20;
	params->maxGen		= 50;
	params->nCrossover	= 1;
	params->pCrossover_s	= 1;
	params->pCrossover_e	= 0.8;
	params->pMutation_s	= 0.02;
	params->pMutation_e	= 0.0001;
	params->elitesize_s	= 1;
	params->elitesize_e	= 15;
	params->ord = 1;

}

static void set_io(param_t* params)
{	
	sprintf(params->in_clauses, "../uf200/uf200-0%d.txt", params->ord);
	sprintf(params->out_best_cost, "../result/uf200_best_cost_0%d.txt", params->ord);
	sprintf(params->out_avg_fun_eval, "../result/uf200_avg_best_cost_func_eval_0%d.txt", params->ord);
	sprintf(params->out_min_hit, "../result/min_hit_0%d.txt", params->ord);
	sprintf(params->out_cpu_time, "../result/cpu_time_0%d.txt", params->ord);
	sprintf(params->out_best_sol, "../result/best_sol_0%d.txt", params->ord);
	sprintf(params->out_fittest, "../result/fittest_0%d.txt", params->ord);
}


static void print_usage()
{
	param_t params;
	default_params(&params);
	fprintf(stderr, 
			"nbody\n"
			"\t-h: print this message\n"
			"\t-t: number of trials (%d)\n"
			"\t-p: size of population in each generation (%d)\n"
			"\t-g: number of generations (%d)\n"
			"\t-n: number of crossover points (%d)\n"
			"\t-C: probability of crossover at the beggining (%g)\n"
			"\t-c: probability of crossover at the end (%g)\n"
			"\t-M: probability of mutation at the begging (%g)\n"
			"\t-m: probability of mutation at the end (%g)\n"
			"\t-E: size of elites to bring to next generation at the beginning (%d)\n"
			"\t-e: size of elites to bring to next generation at the end (%d)\n"
			"\t-o: ordinary of the input clauses file (%d)\n",
			params.trials, params.popsize, params.maxGen, params.nCrossover,
			params.pCrossover_s, params.pCrossover_e, params.pMutation_s, 
			params.pMutation_e, params.elitesize_s, params.elitesize_e,
			params.ord);
}

int set_params(int argc, char** argv, param_t* params)
{
    extern char* optarg;
    const char* optstring = "ht:p:g:n:C:c:M:m:E:e:o:";
    int c;

    #define get_int_arg(c, field) \
        case c: params->field = atoi(optarg); break
    #define get_flt_arg(c, field) \
        case c: params->field = (float) atof(optarg); break

    default_params(params);
    while ((c = getopt(argc, argv, optstring)) != -1) {
        switch (c) {
        case 'h': 
            print_usage(); 
            return -1;
        get_int_arg('t', trials);
        get_int_arg('p', popsize);
        get_flt_arg('g', maxGen);
        get_flt_arg('n', nCrossover);
        get_flt_arg('C', pCrossover_s);
        get_flt_arg('c', pCrossover_e);
        get_flt_arg('M', pMutation_s);
        get_flt_arg('m', pMutation_e);
        get_flt_arg('E', elitesize_s);
        get_flt_arg('e', elitesize_e);
        get_flt_arg('o', ord);
        default:
            fprintf(stderr, "Unknown option\n");
            return -1;
        }
    }

    set_io(params);
    return 0;
}
