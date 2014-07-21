/*
 * qrs_arithmetics.h
 *
 *  Created on: Jul 21, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_ARITHMETICS_H_
#define QRS_ARITHMETICS_H_

#include "qrs_definitions.h"

int qrs_calc_mean_value(int * input, int length);

qrs_status qrs_vector_sub_const(int * input, int length, int value);

qrs_status qrs_vector_mul(int * in_out, int * input, int length);

int qrs_vector_median(int * input, int length);

#endif /* QRS_ARITHMETICS_H_ */
