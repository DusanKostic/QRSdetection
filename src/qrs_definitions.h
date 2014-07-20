/*
 * qrs_definitions.h
 *
 *  Created on: Jul 19, 2014
 *      Author: Dusan Kostic
 */

#ifndef QRS_DEFINITIONS_H_
#define QRS_DEFINITIONS_H_

#define QRS_DEF_FREQUENCY  (256)
#define QRS_DEF_FILT_SIZE  (7)
#define QRS_MEDFILT_WINDOW (10)

typedef struct
{
    int * data;
    int * time_axis;
    int frequency;
    int length;
}qrs_signal;

typedef enum
{
    qrs_no_err,
    qrs_general_err,
    qrs_memalloc_err,
    qrs_fileio_err,
    qrs_bad_arg_err
}qrs_status;

#endif /* QRS_DEFINITIONS_H_ */
