/*
 * qrs_detection.c
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */
#include <stdio.h>
#include <stdlib.h>
#include "qrs_definitions.h"
#include "qrs_detection.h"

static qrs_status qrs_calc_mean_value(int * input, int length, int * output);

static qrs_status qrs_vector_sub_const(int * input, int length, int value);

static qrs_status qrs_differentiate_vector(int * input, int * output,
                                           int input_length);

static qrs_status qrs_copy_vector(int * input, int * output, int length);

qrs_status qrs_detection_core(qrs_signal * input, qrs_signal * output)
{
    qrs_status status = qrs_no_err;
    int mean_value;
    int * diff_data = NULL;
    int diff_length;

    if (input == NULL || output == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    /* Find mean value of input signal */
    status = qrs_calc_mean_value(input->data, input->length, &mean_value);
    if (status != qrs_no_err)
    {
        printf("Calc mean value of input signal failed.\n");
        goto bail;
    }

    /* Remove mean from input signal */
    status = qrs_vector_sub_const(input->data, input->length, mean_value);
    if (status != qrs_no_err)
    {
        printf("Remove mean from input signal failed.\n");
        goto bail;
    }

    /* Differentiate data */
    diff_length = input->length - 1;
    diff_data = malloc(diff_length * sizeof(int));

    status = qrs_differentiate_vector(input->data, diff_data, input->length);
    if (status != qrs_no_err)
    {
        printf("Differentiate input data failed.\n");
        goto bail;
    }

    /* Copy result to output signal */
    status  = qrs_copy_vector(diff_data, output->data, diff_length);
    status |= qrs_copy_vector(input->time_axis, output->time_axis, diff_length);
    if (status != qrs_no_err)
    {
        printf("Copy results to output failed.\n");
        goto bail;
    }
    output->length = diff_length;

bail:
    if (diff_data != NULL)
    {
        free(diff_data);
    }
    return status;
}

static qrs_status qrs_calc_mean_value(int * input, int length, int * output)
{
    qrs_status status = qrs_no_err;
    long int sum = 0;
    int i;

    if (input == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    for(i = 0; i < length; ++i)
    {
        sum += input[i];
    }

    *output = sum / (float)length;

bail:
    return status;
}

static qrs_status qrs_vector_sub_const(int * input, int length, int value)
{
    qrs_status status = qrs_no_err;
    int i;

    if (input == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    for(i = 0; i < length; ++i)
    {
        input[i] -= value;
    }

bail:
    return status;
}

static qrs_status qrs_differentiate_vector(int * input, int * output,
                                           int input_length)
{
    qrs_status status = qrs_no_err;
    int i;

    if (input == NULL || output == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    for(i = 0; i < input_length - 1; ++i)
    {
        output[i] = input[i + 1] - input[i];
    }

bail:
    return status;
}

static qrs_status qrs_copy_vector(int * input, int * output, int length)
{
    qrs_status status = qrs_no_err;
    int i;

    if (input == NULL || output == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    for(i = 0; i < length; ++i)
    {
        output[i] = input[i];
    }

bail:
    return status;
}
