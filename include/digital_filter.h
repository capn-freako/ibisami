// digital_filter.h - interface to DigitalFilter class
//
// Original author: David Banas
// Original date:   May 7, 2015
//
// Copyright (c) 2015 David Banas; all rights reserved World wide.
//
// This class provides a generic digital filter.

#ifndef INCLUDE_DIGITAL_FILTER_H_
#define INCLUDE_DIGITAL_FILTER_H_

#include <vector>

class DigitalFilter {
 public:
    DigitalFilter(const std::vector<double>& num,
                  const std::vector<double>& den);
    virtual ~DigitalFilter() {}
    void apply(double *sig, const long len);

 protected:
    std::vector<double> num_, den_, state_;
    int num_taps_;
};

#endif  // INCLUDE_DIGITAL_FILTER_H_

