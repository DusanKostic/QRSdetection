/*
 * qrs_arithmetics.c
 *
 *  Created on: Jul 21, 2014
 *      Author: Dusan Kostic
 */

#include <stdlib.h>
#include "qrs_arithmetics.h"
#include "qrs_definitions.h"
#include "qrs_utils.h"
#include "qrs_logger.h"

static int cmp_func_qsort (const void * a, const void * b);

int qrs_calc_mean_value(int * input, int length)
{
    long int sum = 0;
    int i;

    for(i = 0; i < length; ++i)
    {
        sum += input[i];
    }

    return sum / (float)length;
}

qrs_status qrs_vector_sub_const(int * input, int length, int value)
{
    qrs_status status = qrs_no_err;
    int i;

    QRS_ASSERT_DO(input != NULL, status = qrs_bad_arg_err,
                  "Input pointer NULL.", bail);

    for(i = 0; i < length; ++i)
    {
        input[i] -= value;
    }

bail:
    return status;
}

qrs_status qrs_vector_mul(int * in_out, int * input, int length)
{
    qrs_status status = qrs_no_err;
    int i;

    QRS_ASSERT_DO(in_out != NULL && input != NULL, status = qrs_bad_arg_err,
                  "Input pointer NULL.", bail);

    for(i = 0; i < length; ++i)
    {
        in_out[i] = in_out[i] * input[i];
    }

bail:
    return status;
}

int qrs_vector_median(int * input, int length)
{
    int odd_length;
    int median;

    odd_length = length % 2;

    qsort(input, length, sizeof(int), cmp_func_qsort);

    if (odd_length)
    {
        median = input[length / 2];
    }
    else
    {
        median = input[length / 2 - 1] + input[length / 2];
        median /= 2;
    }

    return median;
}

static int cmp_func_qsort (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
