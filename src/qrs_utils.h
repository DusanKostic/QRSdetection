/*
 * qrs_utils.h
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_UTILS_H_
#define QRS_UTILS_H_

#include "qrs_definitions.h"
#include "qrs_logger.h"

#define QRS_ASSERT(condition, message, exit) \
                   if (!(condition)){ \
                       qrs_log_error(message); \
                       goto exit; \
                   }

#define QRS_ASSERT_DO(condition, action, message, exit) \
                   if (!(condition)){ \
                       qrs_log_error(message); \
                       action; \
                       goto exit; \
                   }

#define QRS_FREE(x) \
                 if (x){ \
                     free(x); \
                 }

qrs_status qrs_read_signal_from_file(qrs_signal * signal, char * filename);

qrs_status qrs_write_signal_to_file(qrs_signal * signal, char * filename);

qrs_signal * qrs_alloc_signal(int length, int frequency);

void qrs_write_vector_to_file(int * vector, int length, char * filename);

void qrs_free_signal(qrs_signal * signal);

void qrs_copy_vector(int * input, int * output, int length);

#endif /* QRS_UTILS_H_ */
