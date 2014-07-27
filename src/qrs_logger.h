/*
 * qrs_logger.h
 *
 *  Created on: Jul 20, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_LOGGER_H_
#define QRS_LOGGER_H_

#define QRS_PROFILING_START(idx) \
            double t##idx; \
            t##idx = omp_get_wtime(); \

#define QRS_PROFILING_END(tag, idx) \
            t##idx = omp_get_wtime() - t##idx; \
            qrs_write_profiling_data(tag, t##idx);

void qrs_start_logging(char * filename);

void qrs_stop_logging();

void qrs_log_info(char * text);

void qrs_log_debug(char * text);

void qrs_log_error(char * text);

void qrs_write_profiling_data(char tag[], double time);

#endif /* QRS_LOGGER_H_ */
