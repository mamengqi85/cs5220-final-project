#ifndef GA_H
#define GA_H

#include "params.h"
#include "record.h"

void ga(param_t* params, int** Xinitials, record_t* records, int rank, int size, double *t_comm);

#endif /* GA_H */
