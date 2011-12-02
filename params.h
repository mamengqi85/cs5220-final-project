#ifndef PARAMS_H
#define PARAMS_H

typedef struct param_t {
	int trails;			/* number of trails to run					*/
	int len;			/* length of the decision variables			*/
	int popsize;		/* number of individuals in each generation	*/
	int maxGen;			/* number of generations					*/
	int nCrossover;		/* number of crossover points				*/
	float pCrossover_s;	/* starting probability of crossover		*/
	float pCrossover_e;	/* ending probability of crossover			*/
	float pMutation_s;	/* starting probability of mutation			*/
	float pMutation_s;	/* ending probability of mutation			*/
	int elite_size_s;	/* starting number of elites				*/
	int elite_size_e;	/* ending number of elites					*/
	int ord;			/* ordinary of the test set					*/

	int** clauses;		/* clauses to satisfy						*/
	int** n_sat;		/* number of decision valuables in a clause	*/
	int** n_clauses;	/* number of clauses in to formula			*/
} param_t;

int set_params(int argc, char** argv, param_t* params);

#endif /* PARAMS_H */
