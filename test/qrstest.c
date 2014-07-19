/*
 * qrstest.c
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */
#include <stdio.h>
#include <stdlib.h>
#include "qrs_definitions.h"
#include "qrs_utils.h"

/* TODO:
 * Acquire input filename, output filename,
 * length and frequency from command line or ini file.
 */
#define INPUT_FILENAME  "./testdb/rec_1.txt"
#define OUTPUT_FILENAME "./test/out_1.txt"
#define INPUT_LENGTH    (10000)
#define INPUT_FREQUENCY (500)

int main(int argc, char * argv[])
{
    qrs_signal * signal = NULL;
    qrs_status status = qrs_no_err;
    printf("Pan-Tompkins QRS detection algorithm\n");

    signal = qrs_alloc_signal(INPUT_LENGTH, INPUT_FREQUENCY);
    if (signal == NULL)
    {
        printf("Memory allocation failed.\n");
        goto bail;
    }

    printf("Reading input data from file...\n");
    status = qrs_read_signal_from_file(signal, INPUT_FILENAME);
    if (status != qrs_no_err)
    {
        printf("Read signal from file failed.\n");
        goto bail;
    }
    printf("Done.\n");

    printf("Writing output data to file...\n");
    status = qrs_write_signal_to_file(signal, OUTPUT_FILENAME);
    if (status != qrs_no_err)
    {
        printf("Write signal to file failed.\n");
        goto bail;
    }
    printf("Done.\n");

bail:
    qrs_free_signal(signal);

    return 0;
}
