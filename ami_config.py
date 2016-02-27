#! /usr/bin/env python

"""
IBIS-AMI model source code configuration utility.

Original author: David Banas
Original date:   February 26, 2016

This script gets called from a makefile, when a C++ source code file
needs to be built from a EmPy template.

Copyright (c) 2015 David Banas; all rights reserved World wide.
"""

import argparse
import em
# import importlib
import imp

parser = argparse.ArgumentParser(description='Configure IBIS-AMI model source code.')
parser.add_argument('c_file',   metavar='c_file',   type=str, nargs='?', help='name of C++ source file (*.cpp)')
parser.add_argument('em_file',  metavar='em_file',  type=str, nargs='?', help='name of EmPy template file (*.em)')
parser.add_argument('py_file',  metavar='py_file',  type=str, nargs='?', help='name of Python configuration file (*.py)')
args = parser.parse_args()

c_file   = args.c_file
em_file  = args.em_file
py_file  = args.py_file

with open(py_file, 'rt') as cfg_file:
    cfg = imp.load_module(py_file.rsplit('.', 1)[0], cfg_file, py_file, ('py', 'r', imp.PY_SOURCE))

with open(c_file, 'wt') as out_file:
    interpreter = em.Interpreter(
        output = out_file,
        globals = {
            'ami_params'   : cfg.ami_params,
            'c_type_names' : cfg.c_type_names,
            }
        )
    try:
        interpreter.file(open(em_file))
    finally:
        interpreter.shutdown()

