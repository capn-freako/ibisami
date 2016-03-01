#! /usr/bin/env python

"""
Model configurator for ibisami example Tx model.

Original author: David Banas
Original date:   August 19, 2015

The intent is that this example configurator can be used as a starting
point for other custom models.

Copyright (c) 2015 David Banas; all rights reserved World wide.
"""

# import ibisami as ami
# from enum import Enum

kFileBaseName = 'example_tx'
kDescription  = 'Example Tx model from ibisami package.'

ami_params = {
    # These will go in the [Reserved_Parameters] section of the *.AMI file.
    'reserved' : {
        'AMI_Version' : {
            'type'    : 'STRING',
            'default' : '5.1',
            'description' : 'Version of IBIS standard we comply with.',
        },
        'Init_Returns_Impulse' : {
            'type'    : 'BOOL',
            'default' : 'True',
            'description' : "In fact, this model is, currently, Init()-only.",
        },
        'GetWave_Exists' : {
            'type'    : 'BOOL',
            'default' : 'False',
            'description' : "This model is, currently, Init()-only.",
        },
    },

    # These will go in the [Model_Specific] section of the *.AMI file.
    'model' : {
        'tx_tap_units' : {
            'type'    : 'INT',
            'min'     : 6,  # Check this.
            'max'     : 27,
            'default' : 27,
            'tap_pos' : -1,  # Pre-emph. FIR tap position; '-1' means "n/a".
            'description' : 'Total current available to FIR filter.',
        },
        'tx_tap_np1' : {
            'type'    : 'INT',
            'min'     : 6,
            'max'     : 27,
            'default' : 0,
            'tap_pos' : 0,
            'description' : 'First (and only) pre-tap.',
        },
        'tx_tap_nm1' : {
            'type'    : 'INT',
            'min'     : 6,
            'max'     : 27,
            'default' : 0,
            'tap_pos' : 2,
            'description' : 'First post-tap.',
        },
        'tx_tap_nm2' : {
            'type'    : 'INT',
            'min'     : 6,
            'max'     : 27,
            'default' : 0,
            'tap_pos' : 3,
            'description' : 'Second post-tap.',
        },
    },
}

