/** \file example_rx.cpp
 *  \brief Example of using ibisami to build a Rx model.
 *
 * Original author: David Banas <br>
 * Original date:   May 20, 2015
 *
 * Copyright (c) 2015 David Banas; all rights reserved World wide.
 */

#include <string>
#include <vector>
#include "include/ami_rx.h"

#define PI 3.14159
#define RX_BW 30.0e9
#define CTLE_DC_GAIN 1.0

/// An example device specific Rx model implementation.
class MyRx : public AmiRx {
    typedef AmiRx inherited;

 public:
    MyRx() {}
    ~MyRx() {}
    void init(double *impulse_matrix, const long number_of_rows,
            const long aggressors, const double sample_interval,
            const double bit_time, const std::string& AMI_parameters_in) override {

        // Let our base class do its thing.
        inherited::init(impulse_matrix, number_of_rows, aggressors,
            sample_interval, bit_time, AMI_parameters_in);

        // Grab our parameters and configure things accordingly.
        std::vector<std::string> node_names; node_names.clear();

        node_names.push_back("ctle_mode");
        int ctle_mode = get_param_int(node_names, 0);
        node_names.pop_back();
        node_names.push_back("ctle_freq");
        double ctle_freq = get_param_float(node_names, 1.0 / (2.0 * bit_time_));  // Use fNyquist as default.
        node_names.pop_back();
        node_names.push_back("ctle_mag");
        int ctle_mag = get_param_int(node_names, 0);
        node_names.pop_back();

        if (ctle_mode) {
            have_ctle_ = true;

            // Calculate the zero and poles needed to meet the response spec.
            double p2 = -2. * PI * RX_BW;
            double p1 = -2. * PI * ctle_freq;
            double z  = p1 / pow(10., ctle_mag / 20.);

            // Use matched-z transform to convert them to numerator/denominator of transfer function.
            z = exp(z * sample_interval);
            p1 = exp(p1 * sample_interval);
            p2 = exp(p2 * sample_interval);
            num_ = {1.0, -z, 0.0};
            for (auto i = 0; i < num_.size(); i++)
                num_[i] *= (1 - p1) * (1 - p2) * CTLE_DC_GAIN / (1 - z);
            den_ = {1.0, -(p1 + p2), p1 * p2};
        }

        // Fill in params_.
        std::ostringstream params;
        params << "(example_rx";
        params << " (ctle_mode " << ctle_mode << ")";
        params << " (ctle_freq " << ctle_freq << ")";
        params << " (ctle_mag " << ctle_mag << "))";
        param_str_ = params.str() + "\n";
        msg_ += "Hello, World!";
    }
} my_rx;

AMIModel *ami_model = &my_rx;  ///< The pointer required by the API implementation.

