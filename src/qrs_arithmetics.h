/*
 * qrs_arithmetics.h
 *
 *  Created on: Jul 21, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_ARITHMETICS_H_
#define QRS_ARITHMETICS_H_

#include "qrs_definitions.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

int qrs_calc_mean_value(int * input, int length);

qrs_status qrs_vector_sub_const(int * input, int length, int value);

qrs_status qrs_vector_mul(int * in_out, int * input, int length);

int qrs_vector_median(int * input, int length);

int qrs_vector_max(int * input, int length);

int qrs_vector_min(int * input, int length);

int qrs_vector_max_range(int * input, int start, int end);

int qrs_vector_min_range(int * input, int start, int end);

void qrs_vector_minmax_idx_range(int * input, int start, int end,
                                 int * min_idx, int * max_idx);

void qrs_vector_gt_treshold(int * in_out, int length, int treshold);

#endif /* QRS_ARITHMETICS_H_ */
