/*
 * qrs_detection.h
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_DETECTION_H_
#define QRS_DETECTION_H_

#include "qrs_definitions.h"

qrs_status qrs_detection_core(qrs_signal * input, int ** r_peaks_idx,
                              int ** s_peaks_idx, int * peak_number);


#endif /* QRS_DETECTION_H_ */
