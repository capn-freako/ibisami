// ibisami_api.cpp
//
// Original author: David Banas
// Original date:   April 29, 2015
//
// Copyright (c) 2015 David Banas; all rights reserved World wide.
//
// Provides the API (application programming interface) to the *.SO (shared object) or *.DLL (dynamically linked library) file.
//
// Note: The required API for a IBIS-AMI model is defined by the IBIS standard, which is available here:
//       http://www.eda.org/ibis/

extern "C" {
// Initialization and impulse response processing.
long AMI_Init( // Required.
        double * impulse_matrix,
        long     number_of_rows,
        long     aggressors,
        double   sample_interval,
        double   bit_time,
        char   * AMI_parameters_in,
        char  ** AMI_parameters_out,
        void  ** AMI_memory_handle,
        char  ** msg
     )
{
}

`ifdef AMI_GETWAVE
// Time domain signal processing.
long AMI_GetWave( // Optional.
        double * wave,
        long     wave_size,
        double * clock_times,
        char  ** AMI_parameters_out,
        void   * AMI_memory
     )
{
}
`endif

// Clean-up.
long AMI_Close( // Required.
        void * AMI_memory
     )
{
}
}

