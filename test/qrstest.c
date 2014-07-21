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
#include "qrs_detection.h"
#include "qrs_logger.h"

/* TODO:
 * Acquire input filename, output filename,
 * length and frequency from command line or ini file.
 */
#define INPUT_FILENAME  "./testdb/rec_1.txt"
#define OUTPUT_FILENAME "./test/out_1.txt"
#define INPUT_LENGTH    (10000)
#define INPUT_FREQUENCY (500)
#define LOG_FILENAME    "./test/qrs_log.txt"

int main(int argc, char * argv[])
{
    qrs_status status = qrs_no_err;
    qrs_signal * input_signal = NULL;
    qrs_signal * output_signal = NULL;

    qrs_start_logging(LOG_FILENAME);

    qrs_log_info("Pan-Tompkins QRS detection algorithm.");

    input_signal  = qrs_alloc_signal(INPUT_LENGTH, INPUT_FREQUENCY);
    output_signal = qrs_alloc_signal(INPUT_LENGTH, INPUT_FREQUENCY);
    QRS_ASSERT(input_signal != NULL && output_signal != NULL,
               "Memory allocation failed.", bail);

    qrs_log_info("Reading input data from file");
    status = qrs_read_signal_from_file(input_signal, INPUT_FILENAME);
    QRS_ASSERT(status == qrs_no_err, "Read signal from file failed.", bail);

    qrs_log_info("QRS detection start.");
    status = qrs_detection_core(input_signal, output_signal);
    QRS_ASSERT(status == qrs_no_err, "QRS detection failed.", bail);

    qrs_log_info("Writing output data to file.");
    status = qrs_write_signal_to_file(output_signal, OUTPUT_FILENAME);
    QRS_ASSERT(status == qrs_no_err, "Write signal to file failed.", bail);

bail:
    qrs_stop_logging();

    qrs_free_signal(input_signal);
    qrs_free_signal(output_signal);

    return 0;
}
