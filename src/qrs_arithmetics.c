/*
 * qrs_arithmetics.c
 *
 *  Created on: Jul 21, 2014
 *      Author: Dusan Kostic
 */

#include <stdlib.h>
#include <math.h>
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

int qrs_vector_max(int * input, int length)
{
    int i;
    int max_elem = input[0];

    for(i = 0; i < length; ++i)
    {
        max_elem = max(max_elem, input[i]);
    }

    return max_elem;
}

int qrs_vector_min(int * input, int length)
{
    int i;
    int min_elem = input[0];

    for(i = 0; i < length; ++i)
    {
        min_elem = min(min_elem, input[i]);
    }

    return min_elem;
}

int qrs_vector_max_range(int * input, int start, int end)
{
    int length = end - start + 1;
    return qrs_vector_max(&input[start], length);
}

int qrs_vector_min_range(int * input, int start, int end)
{
    int length = end - start + 1;
    return qrs_vector_min(&input[start], length);
}

void qrs_vector_minmax_idx_range(int * input, int start, int end,
                                 int * min_idx, int * max_idx)
{
    int i;
    int tmp_min = input[start];
    int tmp_max = input[start];
    int tmp_min_idx = start;
    int tmp_max_idx = start;

    for (i = start + 1; i <= end; ++i)
    {
        if (input[i] < tmp_min)
        {
            tmp_min = input[i];
            tmp_min_idx = i;
        }
        if (input[i] > tmp_max)
        {
            tmp_max = input[i];
            tmp_max_idx = i;
        }
    }
    *min_idx = tmp_min_idx;
    *max_idx = tmp_max_idx;
}

void qrs_vector_gt_treshold(int * in_out, int length, int treshold)
{
    int i;

    for(i = 0; i < length; ++i)
    {
        in_out[i] = in_out[i] > treshold ? 1 : 0;
    }
}
