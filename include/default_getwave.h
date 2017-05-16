/** \file default_getwave.h
*   \brief Default implementation of IBIS-AMI GetWave().
*
* Original author: David Banas <br>
* Original date:   May 13, 2017
*
* Copyright (c) 2017 David Banas; all rights reserved World wide.
*
* Note: For licensing terms, please, read the LICENSE file for the
*       *ibisami* project, here: https://github.com/capn-freako/ibisami.
*
* Provides a default implementation for the 'AMI_GetWave()' function
* of IBIS-AMI models, so that Init-only models can be converted to
* "dual mode" with minimal effort.
*
* Note: *AMI_Init()* must be modified to store a copy of the processed
*       impulse response, in order to use this function reliably, as we
*       cannot assume the tool will leave the original unmodified.
*
*       For a C++ example, see:
*         - ibisami_api.cpp:AMI_Init()
*         - amimodel.cpp:AMIModel::{init(),save_imp()}
*
*       For a C example, see:
*         - init_example.c
*
* Note that, while 'init_example.c' compiles without error, it has not
* been functionally tested. All testing of the new 'default_getwave()'
* function has been performed using the existing infrastructure, which
* is written in C++.
*/

/// Default implementation of IBIS-AMI GetWave() function.
/**
* Inputs:
*  - signal:        A pointer to a vector of doubles representing the
*                   incoming signal to be modified in place.
*
*  - sig_len:       The length of the incoming signal vector.
*
*  - impulse_resp:  A pointer to a vector of doubles representing the
*                   impulse response of the channel being modeled.
*                   (Presumably, the model's *AMI_Init()* function
*                   stores a copy of the processed impulse response,
*                   just before returning. A pointer to that copy is
*                   what should be passed in, here.)
*                   Note that this version of the impulse response must
*                   be pre-normalized to the sample rate.
*
*  - imp_res_len:   The length of the impulse response.
*
*  - residual:      A pointer to a vector of doubles containing the
*                   last N elements of the previous signal, where N
*                   equals *imp_res_len*.
*
* Returns:
*  - An integer flag indicating success (1) or failure (0).
*
* Throws:
*  - Nothing; this is pure C.
*/

#ifdef __cplusplus
extern "C" {
#endif
int default_getwave(double *signal, long sig_len, double *impulse_resp, long imp_res_len, double *residual);
#ifdef __cplusplus
}
#endif

