/*
 * qrs_logger.h
 *
 *  Created on: Jul 20, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_LOGGER_H_
#define QRS_LOGGER_H_

void qrs_start_logging(char * filename);

void qrs_stop_logging();

void qrs_log_info(char * text);

void qrs_log_debug(char * text);

void qrs_log_error(char * text);

#endif /* QRS_LOGGER_H_ */
