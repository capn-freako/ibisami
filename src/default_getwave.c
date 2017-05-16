/*! \file default_getwave.c
*   \brief Default implementation of IBIS-AMI GetWave().
*
* Original author: David Banas <br>
* Original date:   May 13, 2017
*
* Copyright (c) 2017 David Banas; all rights reserved World wide.
*/

int default_getwave(double *signal, long sig_len, double *impulse_resp, long imp_res_len, double *residual) {
    double proc_vec[imp_res_len + sig_len], accum;
    long   i, j;

    // Prepare the processing vector, by appending the new signal to the tail of the old.
    for (i = 0L; i < imp_res_len; i++)
        proc_vec[i] = residual[i];
    for (i = 0L; i < sig_len; i++)
        proc_vec[imp_res_len + i] = signal[i];

    // Save the new residual tail.
    for (i = 0L; i < imp_res_len; i++)
        residual[i] = proc_vec[sig_len + i];

    // Do the convolution.
    for (i = 0L; i < sig_len; i++) {
        accum = 0.0;
        for (j = 0L; j < imp_res_len; j++)
            accum += proc_vec[i + j] * impulse_resp[imp_res_len - j - 1];
        signal[i] = accum;
    }

    return 1;
}

