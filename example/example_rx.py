#! /usr/bin/env python

"""
Model configurator for ibisami example Rx model.

Original author: David Banas
Original date:   March 24, 2016

The intent is that this example configurator can be used as a starting
point for other custom models.

Copyright (c) 2016 David Banas; all rights reserved World wide.
"""

kFileBaseName = 'example_rx'
kDescription  = 'Example Rx model from ibisami package.'

# These are used to configure the IBIS model.
#
# Lists should contain 3 values:
# typical, min/slow/weak, and max/fast/strong, in that order.
# (The singular exception is 'c_comp', which should contain 3 values,
# in NUMERICAL typ, min, max order.)
ibis_params = {
    'file_name'          : kFileBaseName + '.ibs',
    'file_rev'           : 'v0.1',
    'copyright'          : "Copyright (c) 2016 David Banas; all rights reserved World wide.",
    'component'          : "Example_Rx",
    'manufacturer'       : "(n/a)",
    'r_pkg'              : [0.1,    0.001,    0.5],
    'l_pkg'              : [10.e-9, 0.1e-9,   50.e-9],
    'c_pkg'              : [1.e-12, 0.01e-12, 5.e-12],
    'model_name'         : kFileBaseName,
    'model_type'         : 'Input',
    'c_comp'             : [1.e-12, 0.01e-12, 5.e-12],
    'v_meas'             : 0.5,
    'temperature_range'  : [25,   0,    100],
    'voltage_range'      : [1.8,  1.62, 1.98],
    'impedance'          : [50.,  45.,  55.],
}

# These are used to configure the AMI model.
ami_params = {
    # These will go in the [Reserved_Parameters] section of the *.AMI file.
    'reserved' : {
        'AMI_Version' : {
            'type'    : 'STRING',
            'usage'   : 'Info',
            'format'  : 'Value',
            'default' : '5.1',
            'description' : 'Version of IBIS standard we comply with.',
        },
        'Init_Returns_Impulse' : {
            'type'    : 'BOOL',
            'usage'   : 'Info',
            'format'  : 'Value',
            'default' : 'True',
            'description' : "In fact, this model is, currently, Init()-only.",
        },
        'GetWave_Exists' : {
            'type'    : 'BOOL',
            'usage'   : 'Info',
            'format'  : 'Value',
            'default' : 'False',
            'description' : "This model is, currently, Init()-only.",
        },
    },

    # These will go in the [Model_Specific] section of the *.AMI file.
    'model' : {
        'dfe_ntaps' : {
            'type'    : 'INT',
            'usage'   : 'In',
            'format'  : 'VALUE',
            'default' : 5,
            'description' : 'Number of DFE taps.',
        },
        'dfe_tap1' : {
            'type'    : 'INT',
            'usage'   : 'In',
            'format'  : 'VALUE',
            'default' : 0,
            'description' : 'DFE tap 1 initial value.',
        },
    },
}

