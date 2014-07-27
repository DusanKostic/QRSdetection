/*
 * qrstest.c
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "qrs_definitions.h"
#include "qrs_utils.h"
#include "qrs_detection.h"
#include "qrs_logger.h"

/* TODO:
 * Acquire input filename, output filename,
 * length and frequency from command line or ini file.
 */
#define INPUT_FILENAME  "./testdb/rec_1.txt"
#define RPEAKS_FILENAME "./test/rpeaks_1.txt"
#define SPEAKS_FILENAME "./test/speaks_1.txt"
#define INPUT_LENGTH    (10000)
#define INPUT_FREQUENCY (500)
#define LOG_FILENAME    "./test/qrs_log.txt"

int main(int argc, char * argv[])
{
    qrs_status status = qrs_no_err;
    qrs_signal * input_signal = NULL;
    int * r_peak_idx = NULL;
    int * s_peak_idx = NULL;
    int peak_number;
    int length;
    int frequency;
    char * input_file;
    char * output_file_r;
    char * output_file_s;
    char * log_file;

    if (argc == 7)
    {
        input_file = argv[1];
        output_file_r = argv[2];
        output_file_s = argv[3];
        log_file = argv[4];
        length = atoi(argv[5]);
        frequency = atoi(argv[6]);
    }
    else
    {
        input_file = INPUT_FILENAME;
        output_file_r = RPEAKS_FILENAME;
        output_file_s = SPEAKS_FILENAME;
        log_file = LOG_FILENAME;
        length = INPUT_LENGTH;
        frequency = INPUT_FREQUENCY;
    }

    qrs_start_logging(log_file);

    qrs_log_info("Pan-Tompkins QRS detection algorithm.");

    input_signal  = qrs_alloc_signal(length, frequency);
    QRS_ASSERT(input_signal != NULL, "Memory allocation failed.", bail);

    status = qrs_read_signal_from_file(input_signal, input_file);
    QRS_ASSERT(status == qrs_no_err, "Read signal from file failed.", bail);

    qrs_log_info("QRS detection start.");

    QRS_PROFILING_START(0);

    status = qrs_detection_core(input_signal, &r_peak_idx,
                                &s_peak_idx, &peak_number);

    QRS_PROFILING_END("Total time", 0);

    QRS_ASSERT(status == qrs_no_err, "QRS detection failed.", bail);

    qrs_log_info("QRS detection done.");

    qrs_write_vector_to_file(r_peak_idx, peak_number, output_file_r);
    qrs_write_vector_to_file(s_peak_idx, peak_number, output_file_s);

bail:
    qrs_stop_logging();

    qrs_free_signal(input_signal);
    QRS_FREE(r_peak_idx);
    QRS_FREE(s_peak_idx);

    return 0;
}
