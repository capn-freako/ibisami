#! /usr/bin/env python
# A quick test of the ibisami example Tx model.
#
# Original author: David Banas
# Original date:   May 18, 2015
#
# Copyright (c) 2015 David Banas; all rights reserved World wide.

import pyibisami.ami_model as ami

gDLLName = './example_tx_x86_amd64.so'

the_model = ami.AMIModel(gDLLName)
init_data = ami.AMIModelInitializer({'root_name': "exampleTx"})
the_model.initialize(init_data)

print "Model returned message:", the_model.msg
print "Model returned parameters:", the_model.ami_params_out

