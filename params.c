#include <stdio.h>
#include <stdlib.h>
#include "params.h"

static void default_params(param_t* params)
{
	int ord = 1;

	params->trails		= 30;
	params->len			= 200;
	params->popsize		= 20;
	params->maxGen		= 50;
	params->pCrossover = 0.95;
	params->pMutation	= 0.02;
	params->elitesize	= 1;

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
			"\t-c: probability of crossover (%g)\n"
			"\t-m: probability of mutation (%g)\n"
			"\t-e: size of elites to bring to next generation (%d)\n"
			"\t-o: ordinary of the input clauses file (%d)\n"
			params.trials, params.popsize, params.maxGen,
			params.pCrossover, params.pMutation, params.elitesize);
}

int set_params(int argc, char** argv, param_t* params)
{
    extern char* optarg;
    const char* optstring = "ht:p:g:c:m:e:o:";
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
        case 'o':
            strcpy(params->fname = malloc(strlen(optarg)+1), optarg);
            break;
        get_int_arg('F', nframes);
        get_int_arg('f', npframe);
        get_flt_arg('t', dt);
        get_flt_arg('s', h);
        get_flt_arg('d', rho0);
        get_flt_arg('k', k);
        get_flt_arg('v', mu);
        get_flt_arg('g', g);
        default:
            fprintf(stderr, "Unknown option\n");
            return -1;
        }
    }
    return 0;
}
