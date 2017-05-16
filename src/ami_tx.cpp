/** \file ami_tx.cpp
 *  \brief Implementation of AmiTx class.
 *
 * Original author: David Banas <br>
 * Original date:   May 7, 2015
 *
 * Copyright (c) 2015 David Banas; all rights reserved World wide.
 */

#include <string>
#include "include/ami_tx.h"

/// Process the channel impulse response.
void AmiTx::proc_imp() {
    // Set up the preemphasis filter, if appropriate.
    if (have_preemph_) {
        std::vector<double> den; den.clear(); den.push_back(1.0);
        filter_ = new DigitalFilter(tap_weights_, den);
        if (!filter_) {
            std::ostringstream err;
            err << "AmiTx::init() could not allocate a DigitalFilter with "
            << num_taps_ << " taps having first weight: " << tap_weights_[0] << "\n";
            std::string err_str = err.str();
            throw std::runtime_error(err_str);
        }
        filter_->apply(impulse_matrix_, number_of_rows_);
    }
}

/*
bool AmiTx::proc_sig(double *sig, long len, double *clock_times) {
    filter_->apply(sig, len);
    return true;
}
*/

