/*
 * qrs_logger.c
 *
 *  Created on: Jul 20, 2014
 *      Author: Dusan Kostic
 */
#include <stdio.h>

static FILE * log_file;

void qrs_start_logging(char * filename)
{
    log_file = fopen(filename, "w");
}

void qrs_log_info(char * text)
{
    fprintf(log_file, "[I] %s\n", text);
}

void qrs_log_debug(char * text)
{
    fprintf(log_file, "[D] %s\n", text);
}

void qrs_log_error(char * text)
{
    fprintf(log_file, "[E] %s\n", text);
}

void qrs_write_profiling_data(char tag[], double time)
{
    char text[200];
    int time_ms = (int)(time * 1000);
    sprintf(text, "%s: consumed %d ms", tag, time_ms);
    qrs_log_info(text);
}

void qrs_stop_logging()
{
    if (log_file)
    {
        fclose(log_file);
    }
}
