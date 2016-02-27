/** \file example_tx.cpp
 *  \brief Example of using ibisami to build a Tx model.
 *
 * Original author: David Banas <br>
 * Original date:   May 8, 2015
 * Initial conversion to EmPy template format: Feb 25, 2016
 *
 * Copyright (c) 2015 David Banas; all rights reserved World wide.
 */

#define TAP_SCALE 0.047

#include <string>
#include <vector>
#include "include/ami_tx.h"

/// An example device specific Tx model implementation.
class MyTx : public AmiTx {
    typedef AmiTx inherited;

 public:
    MyTx() {}
    ~MyTx() {}
    void init(double *impulse_matrix, const long number_of_rows,
            const long aggressors, const double sample_interval,
            const double bit_time, const std::string& AMI_parameters_in) override {

        // Let our base class do its thing.
        inherited::init(impulse_matrix, number_of_rows, aggressors,
            sample_interval, bit_time, AMI_parameters_in);

        // Grab our parameters and configure things accordingly.
        std::vector<std::string> node_names; node_names.clear();
        std::ostringstream msg;

        msg = "Initializing Tx...\n";

       