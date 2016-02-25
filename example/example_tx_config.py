#! /usr/bin/env python

"""
Model configurator for ibisami example Tx model.

Original author: David Banas
Original date:   August 19, 2015

The intent is that this example configurator can be used as a starting
point for other custom models.

Copyright (c) 2015 David Banas; all rights reserved World wide.
"""

#import ibisami as ami
from enum import Enum

kFileBaseName = 'example_tx'

class ParamType(Enum):
    INT, FLOAT, BOOL, STRING = range(4)

ami_params = {
    # These will go in the [Reserved] section of the *.AMI file.
    'reserved' : {
        'ami_ver' : '5.1',
    },

    # These are used by the template to configure a generic model,
    # in order to represent a specific device.
    # They may generate entries in the Model_Specific section of the
    # *.AMI file.
    # Change numerical values, as needed, to configure the model to
    # your device, but DON'T CHANGE ANY NAMES, or you will break the
    # flow!
    'model' : {
        'tx_tap_weights' : [[0., 0.05, 0.10, 0.15, 0.20, 0.25],
                            [],
                            [],
                           ],
    },

    # These will go in the [Model Specific] section of the *.AMI file.
    'user' : {
        'tx_tap_units' : {
#            'type'    : ami.ParamType.INT,
            'type'    : ParamType.INT,
            'min'     : 6,  # Check this.
            'max'     : 27,
            'default' : 27,
        }
    },
}

# Standard Python trick, used to allow a single file to be either:
#  - run stand alone, or
#  - imported into a larger project.
if __name__ == '__main__':
    make_ami_model(kFileBaseName, ami_params)

