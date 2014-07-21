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
#include "qrs_logger.h"
#include "qrs_utils.h"
#include "qrs_arithmetics.h"

static qrs_status qrs_differentiate_vector(int * input, int * output,
                                           int input_length);

static qrs_status qrs_integrate_over_window(int * input, int * output,
                                            int length, int frequency);

static qrs_status qrs_filter_1d(int * input, int * output, int length,
                                int * kernel, int kernel_length, int scale);

static qrs_status qrs_median_filter_1d(int * input, int * output,
                                       int length, int window_length);

qrs_status qrs_detection_core(qrs_signal * input, qrs_signal * output)
{
    qrs_status status = qrs_no_err;
    int mean_value;
    int * diff_data = NULL;
    int diff_length;

    QRS_ASSERT_DO(input != NULL && output != NULL, status = qrs_bad_arg_err,
                  "Bad input arguments - NULL pointer.", bail);

    /* Find mean value of input signal */
    mean_value = qrs_calc_mean_value(input->data, input->length);

    /* Remove mean from input signal */
    status = qrs_vector_sub_const(input->data, input->length, mean_value);
    QRS_ASSERT(status == qrs_no_err, "Remove mean from input failed.", bail);

    /* Differentiate data */
    diff_length = input->length - 1;
    diff_data = malloc(diff_length * sizeof(int));
    QRS_ASSERT(diff_data != NULL, "Allocation failed.", bail);

    status = qrs_differentiate_vector(input->data, diff_data, input->length);
    QRS_ASSERT(status == qrs_no_err, "Differentiate input data failed.", bail);

    /* Square differentiated data */
    status = qrs_vector_mul(diff_data, diff_data, diff_length);
    QRS_ASSERT(status == qrs_no_err, "Square data failed.", bail);

    /* Integrate data over window */
    status = qrs_integrate_over_window(diff_data, output->data, diff_length,
                                       input->frequency);
    QRS_ASSERT(status == qrs_no_err, "Integration of data failed.", bail);

    /* Copy result to output signal */
    output->length = diff_length;
    qrs_copy_vector(input->time_axis, output->time_axis, diff_length);

bail:
    QRS_FREE(diff_data);

    return status;
}

static qrs_status qrs_differentiate_vector(int * input, int * output,
                                           int input_length)
{
    qrs_status status = qrs_no_err;
    int i;

    QRS_ASSERT_DO(input != NULL && output != NULL, status = qrs_bad_arg_err,
                  "Input pointer NULL.", bail);

    for(i = 0; i < input_length - 1; ++i)
    {
        output[i] = input[i + 1] - input[i];
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

    QRS_ASSERT_DO(input != NULL && output != NULL, status = qrs_bad_arg_err,
                  "Input pointer NULL.", bail);

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
    QRS_ASSERT_DO(temp_buff != NULL && kernel != NULL,
                  status = qrs_memalloc_err,
                  "Allocation failed.", bail);

    for(i = 0; i < kernel_length; ++i)
    {
        kernel[i] = 1;
    }
    status = qrs_filter_1d(input, temp_buff, length, kernel, kernel_length, 1);
    QRS_ASSERT(status == qrs_no_err, "Box filter failed.", bail);

    /* Median filtering of input data over window */
    status = qrs_median_filter_1d(temp_buff, output, length, QRS_MEDFILT_WINDOW);
    QRS_ASSERT(status == qrs_no_err, "Median filter failed.", bail);

bail:
    QRS_FREE(kernel);
    QRS_FREE(temp_buff);

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

    QRS_ASSERT_DO(input != NULL && output != NULL && kernel != NULL,
                  status = qrs_bad_arg_err, "Input pointer NULL.", bail);

    QRS_ASSERT_DO(length > kernel_length, status = qrs_bad_arg_err,
                  "Invalid length and kernel length.", bail);

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

    QRS_ASSERT_DO(input != NULL && output != NULL,
                  status = qrs_bad_arg_err, "Input pointer NULL.", bail);

    window = malloc(window_length * sizeof(int));
    QRS_ASSERT_DO(window != NULL, status = qrs_memalloc_err,
                  "Allocation failed.", bail);

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
    QRS_FREE(window);

    return status;
}
