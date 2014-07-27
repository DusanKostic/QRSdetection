/*
 * qrs_detection.c
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */
#include <omp.h>
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
                                            int length, int frequency,
                                            int * delay);

static qrs_status qrs_filter_1d(int * input, int * output, int length,
                                int * kernel, int kernel_length, int scale);

static qrs_status qrs_median_filter_1d(int * input, int * output,
                                       int length, int window_length);

static qrs_status
qrs_find_indices_into_boundaries(int * input, int length,
                                 int ** left_indices, int ** right_indices,
                                 int * left_length, int * right_length);

static void check_and_realloc(int ** input, int curr_cnt, int * curr_length);

static void swap_pointers(int ** a, int ** b);

qrs_status qrs_detection_core(qrs_signal * input, int ** r_peaks_idx,
                              int ** s_peaks_idx, int * peak_number)
{
    qrs_status status = qrs_no_err;
    int mean_value;
    int * diff_data = NULL;
    int * integral_data = NULL;
    int * placeholder_ptr = NULL;
    int * left_indices = NULL;
    int * right_indices = NULL;
    int integral_length;
    int diff_length;
    int delay;
    int max_value;
    int start, end;
    int treshold;
    int left_ind_len;
    int right_ind_len;
    int i;

    QRS_ASSERT_DO(input != NULL, status = qrs_bad_arg_err,
                  "Bad input arguments - NULL pointer.", bail);

    /* Find mean value of input signal */
    QRS_PROFILING_START(0);
    mean_value = qrs_calc_mean_value(input->data, input->length);
    QRS_PROFILING_END("Mean value", 0);

    /* Remove mean from input signal */
    QRS_PROFILING_START(1);
    status = qrs_vector_sub_const(input->data, input->length, mean_value);
    QRS_PROFILING_END("Remove mean", 1);
    QRS_ASSERT(status == qrs_no_err, "Remove mean from input failed.", bail);

    /* Differentiate data */
    QRS_PROFILING_START(2);
    diff_length = input->length - 1;
    diff_data = malloc(diff_length * sizeof(int));
    QRS_ASSERT(diff_data != NULL, "Allocation failed.", bail);

    status = qrs_differentiate_vector(input->data, diff_data, input->length);
    QRS_PROFILING_END("Differentiate", 2);
    QRS_ASSERT(status == qrs_no_err, "Differentiate input data failed.", bail);

    /* Square differentiated data */
    QRS_PROFILING_START(3);
    status = qrs_vector_mul(diff_data, diff_data, diff_length);
    QRS_PROFILING_END("Square", 3);
    QRS_ASSERT(status == qrs_no_err, "Square data failed.", bail);

    /* Integrate data over window */
    QRS_PROFILING_START(4);
    integral_data = malloc(diff_length * sizeof(int));
    status = qrs_integrate_over_window(diff_data, integral_data, diff_length,
                                       input->frequency, &delay);
    QRS_PROFILING_END("Integrate", 4);
    QRS_ASSERT(status == qrs_no_err, "Integration of data failed.", bail);

    /* Remove filter delay for scanning back through ECG */
    placeholder_ptr = integral_data;
    integral_data = &integral_data[delay - 1];
    integral_length = diff_length - (delay - 1);

    /* Segment search area, first find the highest bumps */
    QRS_PROFILING_START(5);
    start = round(diff_length / 4.0) - 1;
    end   = round(3 * diff_length / 4.0) - 1;
    max_value = qrs_vector_max_range(integral_data, start, end);
    QRS_PROFILING_END("Segment", 5);

    /* Build an array of segments to look in */
    QRS_PROFILING_START(6);
    treshold = QRS_TRESHOLD * max_value;
    qrs_vector_gt_treshold(integral_data, integral_length, treshold);
    QRS_PROFILING_END("Treshold", 6);

    /* Find indices into boundaries of each segment */
    QRS_PROFILING_START(7);
    status = qrs_find_indices_into_boundaries(integral_data, integral_length,
                                              &left_indices, &right_indices,
                                              &left_ind_len, &right_ind_len);
    QRS_PROFILING_END("Indices", 7);
    QRS_ASSERT(status == qrs_no_err, "Find indices failed.", bail);

     /* Loop through all possibilities */
    QRS_PROFILING_START(8);
    end = min(left_ind_len, right_ind_len);

    *r_peaks_idx = malloc(end * sizeof(int));
    *s_peaks_idx = malloc(end * sizeof(int));
    QRS_ASSERT_DO(*r_peaks_idx != NULL && *s_peaks_idx != NULL,
                  status = qrs_memalloc_err, "Allocation failed.", bail);

    for (i = 0; i < end; ++i)
    {
        qrs_vector_minmax_idx_range(input->data, left_indices[i], right_indices[i],
                                    &(*s_peaks_idx)[i], &(*r_peaks_idx)[i]);
    }
    *peak_number = end;

    if ((*s_peaks_idx)[end - 1] < (*r_peaks_idx)[end - 1])
    {
        swap_pointers(r_peaks_idx, s_peaks_idx);
    }
    QRS_PROFILING_END("Find peaks", 8);

bail:
    QRS_FREE(diff_data);
    QRS_FREE(placeholder_ptr);
    QRS_FREE(left_indices);
    QRS_FREE(right_indices);

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
                                            int length, int frequency,
                                            int * delay)
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

    *delay = ceil(kernel_length / 2.0);

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
    QRS_PROFILING_START(0);
    status = qrs_filter_1d(input, temp_buff, length, kernel, kernel_length, 1);
    QRS_PROFILING_END("Box filter", 0);
    QRS_ASSERT(status == qrs_no_err, "Box filter failed.", bail);

    /* Median filtering of input data over window */
    QRS_PROFILING_START(1);
    status = qrs_median_filter_1d(temp_buff, output, length, QRS_MEDFILT_WINDOW);
    QRS_PROFILING_END("Median filter", 1);
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

static qrs_status
qrs_find_indices_into_boundaries(int * input, int length,
                                 int ** left_indices, int ** right_indices,
                                 int * left_length, int * right_length)
{
    qrs_status status = qrs_no_err;
    int * tmp_left = NULL;
    int * tmp_right = NULL;
    int left_cnt = 0;
    int right_cnt = 0;
    int diff;
    int i;

    *left_length = QRS_INDICES_LENGTH;
    *right_length = QRS_INDICES_LENGTH;

    tmp_left = malloc(*left_length * sizeof(int));
    tmp_right = malloc(*right_length * sizeof(int));
    QRS_ASSERT_DO(tmp_left != NULL && tmp_right != NULL,
                  status = qrs_memalloc_err, "Allocation failed.", bail);

    /* Handle first element of input vector */
    if (input[0] == 1)
    {
        tmp_left[left_cnt] = 0;
        left_cnt++;
    }

    for (i = 1; i < length - 1; ++i)
    {
        diff = input[i] - input[i - 1];
        if (diff == 1)
        {
            tmp_left[left_cnt] = i;
            left_cnt++;
        }
        if (diff == -1)
        {
            tmp_right[right_cnt] = i - 1;
            right_cnt++;
        }
        check_and_realloc(&tmp_left, left_cnt, left_length);
        check_and_realloc(&tmp_right, right_cnt, right_length);
    }

    /* Handle last element of input vector */
    if (input[length - 1] == 1)
    {
        tmp_right[right_cnt] = length - 1;
        right_cnt++;
    }

    *left_indices = tmp_left;
    *right_indices = tmp_right;
    *left_length = left_cnt;
    *right_length = right_cnt;

bail:
    return status;

}

static void check_and_realloc(int ** input, int curr_cnt, int * curr_length)
{
    int new_length;

    if (curr_cnt == *curr_length)
    {
        new_length = *curr_length + QRS_INDICES_LENGTH;
        *input = realloc(*input, new_length * sizeof(int));
        *curr_length = new_length;
    }
}

static void swap_pointers(int ** a, int ** b)
{
    int * tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
