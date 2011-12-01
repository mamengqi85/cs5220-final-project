#ifndef PARAMS_H
#define PARAMS_H

typedef struct param_t {
	float p_crossover;		/* probability of crossover		*/
	float p_mutation;		/* probability of mutation		*/
	int elite_size;			/* number of elites				*/
} param_t;

void set_params(int argc, char** argv, param_t* params);

#endif /* PARAMS_H */
