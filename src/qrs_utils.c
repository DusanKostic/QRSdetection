/*
 * qrs_utils.c
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */
#include <stdio.h>
#include <stdlib.h>
#include "qrs_utils.h"
#include "qrs_definitions.h"

qrs_status qrs_read_signal_from_file(qrs_signal * signal, char * filename)
{
    qrs_status status = qrs_no_err;
    FILE * fp = NULL;
    int tmp_var;
    int i;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        status = qrs_fileio_err;
        goto bail;
    }

    /* NOTE:
     * Test data is in format:
     *     time  raw_signal  filtered_signal
     * For now we are using raw signal, filtered signal
     * is not used.
     */
    for(i = 0; i < signal->length; ++i)
    {
        fscanf(fp, "%d %d %d", &signal->time_axis[i],
                               &signal->data[i], &tmp_var);
    }

    fclose(fp);

bail:
    return status;
}

qrs_status qrs_write_signal_to_file(qrs_signal * signal, char * filename)
{
    qrs_status status = qrs_no_err;
    FILE * fp = NULL;
    int i;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        status = qrs_fileio_err;
        goto bail;
    }

    for(i = 0; i < signal->length; ++i)
    {
        fprintf(fp, "%d  %d\n", signal->time_axis[i], signal->data[i]);
    }

    fclose(fp);

bail:
    return status;
}

qrs_signal * qrs_alloc_signal(int length, int frequency)
{
    qrs_signal * signal = NULL;

    signal = malloc(sizeof(qrs_signal));
    if (signal == NULL)
    {
        goto bail;
    }

    signal->data = malloc(length * sizeof(int));
    if (signal->data == NULL)
    {
        free(signal);
        signal = NULL;
        goto bail;
    }

    signal->time_axis = malloc(length * sizeof(int));
    if (signal->time_axis == NULL)
    {
        free(signal->data);
        free(signal);
        signal = NULL;
        goto bail;
    }

    signal->length = length;
    signal->frequency = frequency;

bail:
    return signal;
}

void qrs_free_signal(qrs_signal * signal)
{
    if (signal == NULL)
    {
        return;
    }

    if (signal->data != NULL)
    {
        free(signal->data);
        signal->data = NULL;
    }
    if (signal->time_axis != NULL)
    {
        free(signal->time_axis);
        signal->time_axis = NULL;
    }

    free(signal);
    signal = NULL;
}
