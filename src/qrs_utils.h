/*
 * qrs_utils.h
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_UTILS_H_
#define QRS_UTILS_H_

#include "qrs_definitions.h"

qrs_status qrs_read_signal_from_file(qrs_signal * signal, char * filename);

qrs_status qrs_write_signal_to_file(qrs_signal * signal, char * filename);


#endif /* QRS_UTILS_H_ */
