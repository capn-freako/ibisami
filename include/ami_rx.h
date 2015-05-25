/** \file ami_rx.h
 *  \brief Interface to AmiRx class.
 *
 * Original author: David Banas <br>
 * Original date:   May 20, 2015
 *
 * Copyright (c) 2015 David Banas; all rights reserved World wide.
 */

#ifndef INCLUDE_AMI_RX_H_
#define INCLUDE_AMI_RX_H_

#include <string>
#include <vector>
#include "include/amimodel.h"
#include "include/digital_filter.h"

/// A generic IBIS-AMI Rx model implementation.
/**
 * This abstract class provides generic Rx model capability.
 * Device specific Rx models should derive from this class.
 */
class AmiRx : public AMIModel {
    typedef AMIModel inherited;
 public:
    virtual ~AmiRx() {delete filter_;}
    void proc_imp() override;

 protected:
    DigitalFilter *filter_;  ///< Used for CTLE.
    bool have_ctle_;  ///< True, if I have a CTLE filter.
    int num_taps_;  ///< Number of taps in my pre-emphasis filter.
    std::vector<double> num_, den_;  ///< Numerator/denominator of frequency response.
};

#endif  // INCLUDE_AMI_RX_H_

