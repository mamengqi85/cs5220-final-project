#include <stdio.h>
#include <stdlib.h>
#include "params.h"

static void default_params(param_t* params)
{
	params->trails		= 30;
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
	sprintf(params->in_clauses, "uf200/uf200-%d.txt", ord);
	sprintf(params->out_best_cost, "uf200_best_cost_%d.txt", ord);
	sprintf(params->out_avg_fun_eval, "uf200_avg_best_cost_func_eval_%d.txt". ord);
	sprintf(params->out_min_hit, "min_hit_%d.txt", ord);
	sprintf(params->out_cpu_time, "cpu_time_%d.txt", ord);
	sprintf(params->out_best_sol, "best_sol_%d.txt", ord);
}


static void print_usage()
{
	param_t params;
	default_params(&params);
	fprintf(stdewrr, 
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
			"\t-o: ordinary of the input clauses file (%d)\n"
			params.trials, params.popsize, params.maxGen, params.nCrossover
			params.pCrossover_s, params.pCrossover_e, params.pMutation_s, 
			params.pMutation_e, params.elitesize_s, params.elitesize_e
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
        get_int_arg('t', trails);
        get_int_arg('p', popsize);
        get_flt_arg('g', maxGen);
        get_flt_arg('n', nCrossover);
        get_flt_arg('C', pCrossover_s);
        get_flt_arg('c', pCrossover_e);
        get_flt_arg('M', pMutation_s);
        get_flt_arg('m', pMutation_e);
        get_flt_arg('E', elite_size_s);
        get_flt_arg('e', elite_size_e);
        get_flt_arg('o', ord);
        default:
            fprintf(stderr, "Unknown option\n");
            return -1;
        }
    }

    set_io(params);
    return 0;
}
