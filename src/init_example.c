/*! \file init_example.c
*   \brief Provides an example of the required additions to AMI_Init(), in C.
*
* Original author: David Banas <br>
* Original date:   May 15, 2017
*
* Copyright (c) 2017 David Banas; all rights reserved World wide.
*
* Note that, while this file has been confirmed to compile without error,
* it has NOT been functionally tested.
*/

// The following is to accomodate Windows and make this file OS agnostic.
// It is taken directly from the Microsoft Visual Studio header file examples.
#ifdef _MSC_VER  // Gets defined, only when the MSVS compiler is invoked.
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include <stdlib.h>
#include "include/default_getwave.h"

static double *impulse_matrix_, *last_sig_tail_;
static long    number_of_rows_;

DLL_EXPORT long AMI_Init(
        double * impulse_matrix,
        long     number_of_rows,
        long     aggressors,
        double   sample_interval,
        double   bit_time,
        char   * AMI_parameters_in,
        char  ** AMI_parameters_out,
        void  ** AMI_memory_handle,
        char  ** msg
    ) {

    impulse_matrix_ = impulse_matrix;
    number_of_rows_ = number_of_rows;

    // {usual init processing here.}

    double *tmp_ptr        = malloc (number_of_rows_ * sizeof(double));
    double *last_sig_tail_ = malloc (number_of_rows_ * sizeof(double));
    if (!tmp_ptr || !last_sig_tail_) {
        // {Handle error.}
        return 0L;
    }
    int i;
    for (i=0; i < number_of_rows_; i++) {
        // Prescaling the stored impulse response saves work in default_getwave().
        tmp_ptr[i]        = impulse_matrix_[i] * sample_interval;
        last_sig_tail_[i] = 0.0;
    }
    impulse_matrix_ = tmp_ptr;

    return 1L;
}

DLL_EXPORT long AMI_GetWave(
        double * wave,
        long     wave_size,
        double * clock_times,
        char  ** AMI_parameters_out,
        void   * AMI_memory
    ) {
  
    clock_times[0] = -1;  // Flags the tool that we've not filled in the 'clock_times' vector.
    if (default_getwave(wave, wave_size, impulse_matrix_, number_of_rows_, last_sig_tail_))
        return 1L;
    else
        return 0L;
}

DLL_EXPORT long AMI_Close(
        void * AMI_memory
    ) {

    // {usual close processing here.}

    free (impulse_matrix_);
    free (last_sig_tail_);
    return 1L;
}

