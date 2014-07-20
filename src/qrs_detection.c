/*
 * qrs_detection.c
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "qrs_definitions.h"
#include "qrs_detection.h"

static qrs_status qrs_calc_mean_value(int * input, int length, int * output);

static qrs_status qrs_vector_sub_const(int * input, int length, int value);

static qrs_status qrs_differentiate_vector(int * input, int * output,
                                           int input_length);

static qrs_status qrs_copy_vector(int * input, int * output, int length);

static qrs_status qrs_vector_mul(int * in_out, int * input, int length);

static qrs_status qrs_integrate_over_window(int * input, int * output,
                                            int length, int frequency);

static qrs_status qrs_filter_1d(int * input, int * output, int length,
                                int * kernel, int kernel_length, int scale);

static qrs_status qrs_median_filter_1d(int * input, int * output,
                                       int length, int window_length);

static int qrs_vector_median(int * input, int length);

static int cmp_func_qsort (const void * a, const void * b);

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

    /* Square differentiated data */
    status = qrs_vector_mul(diff_data, diff_data, diff_length);
    if (status != qrs_no_err)
    {
        printf("Square differentiated data failed.\n");
        goto bail;
    }

    /* Integrate data over window */
    status = qrs_integrate_over_window(diff_data, output->data, diff_length,
                                       input->frequency);
    if (status != qrs_no_err)
    {
        printf("Integration of data failed.\n");
        goto bail;
    }

    /* Copy result to output signal */
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

static qrs_status qrs_vector_mul(int * in_out, int * input, int length)
{
    qrs_status status = qrs_no_err;
    int i;

    if (in_out == NULL || input == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    for(i = 0; i < length; ++i)
    {
        in_out[i] = in_out[i] * input[i];
    }

bail:
    return status;
}

static qrs_status qrs_integrate_over_window(int * input, int * output,
                                            int length, int frequency)
{
    qrs_status status = qrs_no_err;
    int kernel_length;
    int * kernel = NULL;
    int * temp_buff = NULL;
    int i;

    if (input == NULL || output == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    if (frequency >= QRS_DEF_FREQUENCY)
    {
        kernel_length = QRS_DEF_FILT_SIZE * frequency;
        kernel_length = round(kernel_length / (double) QRS_DEF_FREQUENCY);
    }
    else
    {
        kernel_length = QRS_DEF_FILT_SIZE;
    }

    /* Filter input data with box filter of window_size */
    temp_buff = malloc(length * sizeof(int));
    kernel = malloc(kernel_length * sizeof(int));
    if (kernel == NULL || temp_buff == NULL)
    {
        printf("Failed to allocate memory.\n");
        goto bail;
    }
    for(i = 0; i < kernel_length; ++i)
    {
        kernel[i] = 1;
    }
    status = qrs_filter_1d(input, temp_buff, length, kernel, kernel_length, 1);
    if (status != qrs_no_err)
    {
        printf("Box filter failed.\n");
        goto bail;
    }

    /* Median filtering of input data over window */
    status = qrs_median_filter_1d(temp_buff, output, length, QRS_MEDFILT_WINDOW);
    if (status != qrs_no_err)
    {
        printf("Median filter failed.\n");
        goto bail;
    }

bail:
    if (kernel)
    {
        free(kernel);
    }
    if (temp_buff)
    {
        free(temp_buff);
    }
    return status;
}

static qrs_status qrs_filter_1d(int * input, int * output, int length,
                                int * kernel, int kernel_length, int scale)
{
    qrs_status status = qrs_no_err;
    int i, j, k;
    int start;
    int start_kernel;
    int sum;

    if (input == NULL || output == NULL || kernel == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    for(i = 0; i < length; ++i)
    {
        sum = 0;
        start = i >= kernel_length ? i - kernel_length + 1 : 0;
        start_kernel = i >= kernel_length ? 0 : kernel_length - i - 1;

        for(j = start_kernel, k = 0; j < kernel_length; ++j, ++k)
        {
            sum += kernel[j] * input[start + k];
        }
        output[i] = sum / (float) scale;
    }

bail:
    return status;
}

static qrs_status qrs_median_filter_1d(int * input, int * output,
                                       int length, int window_length)
{
    qrs_status status = qrs_no_err;
    int * window = NULL;
    int num_elems_before;
    int start;
    int i, j;

    if (input == NULL || output == NULL)
    {
        status = qrs_bad_arg_err;
        goto bail;
    }

    window = malloc(window_length * sizeof(int));
    if (window == NULL)
    {
        status = qrs_memalloc_err;
        goto bail;
    }
    num_elems_before = window_length / 2 ;

    for(i = 0; i < length; ++i)
    {
        start = i - num_elems_before;
        for(j = 0; j < window_length; ++j)
        {
            if (start + j < 0 || start + j >= length)
            {
                window[j] = 0;
            }
            else
            {
                window[j] = input[start + j];
            }
        }
        output[i] = qrs_vector_median(window, window_length);
    }

bail:
    if (window)
    {
        free(window);
    }
    return status;
}

static int qrs_vector_median(int * input, int length)
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
