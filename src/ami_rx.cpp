/** \file ami_rx.cpp
 *  \brief Implementation of AmiRx class.
 *
 * Original author: David Banas <br>
 * Original date:   May 20, 2015
 *
 * Copyright (c) 2015 David Banas; all rights reserved World wide.
 */

#include <string>
#include "include/ami_rx.h"

/// Process the channel impulse response.
void AmiRx::proc_imp() {
    // Set up the CTLE filter and convolve it with the input impulse response, if appropriate.
    if (have_ctle_) {
        filter_ = new DigitalFilter(num_, den_);
        if (!filter_) {
            std::ostringstream err;
            err << "AmiRx::init() could not allocate a DigitalFilter with "
            << num_taps_ << " taps having first weight: " << num_[0] << "\n";
            std::string err_str = err.str();
            throw std::runtime_error(err_str);
        }
        filter_->apply(impulse_matrix_, number_of_rows_);
    }
}

