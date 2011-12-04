#ifndef PARAMS_H
#define PARAMS_H

typedef struct param_t {
	int trials;			/* number of trials to run					*/
	int len;			/* length of the decision variables			*/
	int popsize;		/* number of individuals in each generation	*/
	int maxGen;			/* number of generations					*/
	int nCrossover;		/* number of crossover points				*/
	float pCrossover_s;	/* starting probability of crossover		*/
	float pCrossover_e;	/* ending probability of crossover			*/
	float pMutation_s;	/* starting probability of mutation			*/
	float pMutation_e;	/* ending probability of mutation			*/
	int elitesize_s;	/* starting number of elites				*/
	int elitesize_e;	/* ending number of elites					*/
	int ord;			/* ordinary of the test set					*/

	int** clauses;		/* clauses to satisfy						*/
	int n_sat;			/* number of decision valuables in a clause	*/
	int n_clauses;		/* number of clauses in to formula			*/

	char* in_clauses;
	char* out_best_cost;
	char* out_avg_fun_eval;
	char* out_min_hit;
	char* out_cpu_time;
	char* out_best_sol;
	char* out_fittest;
} param_t;

param_t* alloc_param();
void free_param(param_t* params);
int set_params(int argc, char** argv, param_t* params);

#endif /* PARAMS_H */
