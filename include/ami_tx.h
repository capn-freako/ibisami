// ami_tx.h - interface to AmiTx class
//
// Original author: David Banas
// Original date:   May 6, 2015
//
// Copyright (c) 2015 David Banas; all rights reserved World wide.
//
// This abstract class provides generic Tx model capability.
// Device specific Tx models should derive from this class.

#ifndef INCLUDE_AMI_TX_H_
#define INCLUDE_AMI_TX_H_

#include <string>
#include <vector>
#include "include/amimodel.h"
#include "include/digital_filter.h"

class AmiTx : public AMIModel {
    typedef AMIModel inherited;
 public:
    virtual ~AmiTx() {delete filter_;}
    void proc_imp() override;

 protected:
    DigitalFilter *filter_;
    bool have_preemph_;
    int num_taps_;
    std::vector<double> tap_weights_;
};

#endif  // INCLUDE_AMI_TX_H_

