/** \file ami_rx.cpp
 *  \brief Implementation of AmiRx class.
 *
 * Original author: David Banas <br>
 * Original date:   May 20, 2015
 *
 * Copyright (c) 2015 David Banas; all rights reserved World wide.
 */

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "include/ami_rx.h"
#include "include/util.h"

#define MAX_INIT_ADAPT_BITS 10000

extern std::ostringstream debug_stream;

/// Initializer
void AmiRx::init(double *impulse_matrix, const long number_of_rows,
                 const long aggressors, const double sample_interval,
                 const double bit_time, const std::string& AMI_parameters_in) {
    // Let our base class do its thing.
    inherited::init(impulse_matrix, number_of_rows, aggressors, sample_interval, bit_time, AMI_parameters_in);

    // Initialize our own state.
    delete ctle_;
    delete dfe_;
    ctle_ = nullptr;
    dfe_ = nullptr;
    dump_dfe_adaptation_ = 0;
    dump_adaptation_input_ = 0;
    sig_tap_ = 0;
    init_adapt_tap_ = 0;

    // Grab debugging related parameters and configure things accordingly.
    std::vector<std::string> node_names; node_names.clear();
    node_names.push_back("debug");
    node_names.push_back("enable");
    int dbg_enable = get_param_int(node_names, 0);
    node_names.pop_back();
    node_names.push_back("sig_tap");
    int sig_tap = get_param_int(node_names, 0);
    node_names.pop_back();
    node_names.push_back("init_adapt_tap");
    int init_adapt_tap = get_param_int(node_names, 0);
    node_names.pop_back();
    node_names.pop_back();
    if (dbg_enable) {
        sig_tap_ = sig_tap;
        init_adapt_tap_ = init_adapt_tap;
    }
    std::ostringstream msg_stream;
    msg_stream << "Debugging parameters:\n";
    msg_stream << "\tenable: " << dbg_enable << "\n";
    msg_stream << "\tsig_tap: " << sig_tap << "\n";
    msg_stream << "\tinit_adapt_tap: " << init_adapt_tap << "\n";
    msg_ += msg_stream.str();
}

/// Override of AMIModel::proc_sig() specific to Rx models.
bool AmiRx::proc_sig(double *sig, long len, double *clock_times) {
    if (ctle_) {
        ctle_->apply(sig, len);
    }
    if (dfe_) {
        return dfe_->apply(sig, len, clock_times);
    } else {
        return true;
    }
}

/// Override of AMIModel::proc_imp() specific to Rx models.
void AmiRx::proc_imp() {
    int bits = 0;
    int bits_per_call = number_of_rows_ / samples_per_bit_;
    long num_whole_bits = number_of_rows_ / samples_per_bit_;
    std::vector<double> dummy_clks, test_sig(num_whole_bits * samples_per_bit_);
    dummy_clks.resize(num_whole_bits);

    if (sig_tap_ == 1) // Return input unmodified.
        return;

    try {
    if (dfe_) {  // If we have a DFE,
        if (dfe_->mode > 1) {  // and it's set to an adaptive mode, then let it adapt.
            // Setup the filter used to convolve binary data with the given impulse response.
            std::vector<double> weights;
            for (auto i = 0; i < number_of_rows_; i++)
                weights.push_back(impulse_matrix_[i] * sample_interval_);
            FIRFilter conv_filt(weights);

            // Generate some random data, and convolve it with the impulse response.
            gen_data(test_sig.data());

            if (init_adapt_tap_ == 1) {  // Return test data.
                int i = 0;
                for (auto x : test_sig)
                    impulse_matrix_[i++] = x;
                return;
            }

            conv_filt.apply(test_sig.data(), num_whole_bits * samples_per_bit_);

            if (init_adapt_tap_ == 2) {  // Return test signal.
                int i = 0;
                for (auto x : test_sig)
                    impulse_matrix_[i++] = x;
                return;
            }

            // Apply CTLE, if appropriate.
            if (ctle_)
                ctle_->apply(test_sig.data(), test_sig.size());

            if (init_adapt_tap_ == 3) {  // Return test signal, after CTLE processing.
                int i = 0;
                for (auto x : test_sig)
                    impulse_matrix_[i++] = x;
                return;
            }

            // Shift resultant signal, such that its zero crossings, on average, occur at integral multiples of the unit interval.
            // - Apply signum function to test signal.
            std::unique_ptr<std::vector<int>> test_sig_sign {new std::vector<int>(test_sig.size())};
            std::transform(test_sig.begin(), test_sig.end(), test_sig_sign->begin(), sgn<double>);
            // - Apply difference function.
            std::unique_ptr<std::vector<int>> test_sig_sign_diff {new std::vector<int>(test_sig.size())};
            std::transform(test_sig_sign->begin() + 1, test_sig_sign->end(), test_sig_sign->begin(), test_sig_sign_diff->begin(), std::minus<int>());
            // - Take the magnitude.
            std::unique_ptr<std::vector<int>> test_sig_sign_diff_mag {new std::vector<int>(test_sig.size())};
            std::transform(test_sig_sign_diff->begin(), test_sig_sign_diff->end(), test_sig_sign_diff_mag->begin(), abs);
            // - Locate all the '2's. These mark the zero crossings in the original signal.
            std::vector<int> zero_xing_indeces;
            auto next_xing = std::find(test_sig_sign_diff_mag->begin(), test_sig_sign_diff_mag->end(), 2);
            while (next_xing != test_sig_sign_diff_mag->end()) {
                zero_xing_indeces.push_back(next_xing - test_sig_sign_diff_mag->begin());
                next_xing = std::find(next_xing + 1, test_sig_sign_diff_mag->end(), 2);
            }
            // - Locate the nearest integral multiple of UI to all the found crossings.
            std::vector<int> zero_xing_refs;
            zero_xing_refs.resize(zero_xing_indeces.size());
            std::transform(zero_xing_indeces.begin(), zero_xing_indeces.end(), zero_xing_refs.begin(),
                    [&](int index){return int(1.0 * index / samples_per_bit_ + 0.5) * samples_per_bit_;});
            // - Calculate the differences between the actual and ideal crossings.
            std::vector<int> zero_xing_diffs;
            zero_xing_diffs.resize(zero_xing_indeces.size());
            std::transform(zero_xing_indeces.begin(), zero_xing_indeces.end(), zero_xing_refs.begin(), zero_xing_diffs.begin(), std::minus<int>());
            // - Find the mean difference.
            long to_shift = std::accumulate(zero_xing_diffs.begin(), zero_xing_diffs.end(), 0) / zero_xing_diffs.size();
            if (to_shift < 0)
                to_shift += samples_per_bit_;
            // - Rotate the original signal accordingly.
            //std::rotate(test_sig.begin(), test_sig.begin() + to_shift, test_sig.end());
            //test_sig.erase(test_sig.end() - to_shift, test_sig.end());
            std::rotate(test_sig.begin(), test_sig.begin() + 4, test_sig.end());
            test_sig.erase(test_sig.end() - 4, test_sig.end());
            test_sig.resize(number_of_rows_, test_sig.back());

            if (init_adapt_tap_ == 4) {  // Return test signal, just before adaptation.
                int i = 0;
                for (auto x : test_sig)
                    impulse_matrix_[i++] = x;
                return;
            }

            // Dump the resultant waveform, if requested.
            if (dump_adaptation_input_) {
                dfe_input_stream_.open(dfe_input_file_);
                if (dfe_input_stream_.fail()) {
                    throw std::runtime_error("ERROR: AmiRx::proc_imp(): Could not open DFE input dump file!");
                }
                dfe_input_stream_ << "Time, DFE Input\n";
                for (auto i = 0; i < number_of_rows_; i++) {
                    dfe_input_stream_ << i * sample_interval_ << ", " << test_sig[i] << "\n";
                }
                dfe_input_stream_.close();
            }

            // Call apply() repeatedly, until we exhaust MAX_INIT_ADAPT_BITS, or adaptation converges.
            if (dump_dfe_adaptation_) {
                dfe_dump_stream_.open(dfe_dump_file_);
                if (dfe_dump_stream_.fail()) {
                    throw std::runtime_error("ERROR: AmiRx::proc_imp(): Could not open DFE adaptation dump file!");
                }
                std::vector<double> tmp_weights = dfe_->get_weights();
                for (auto i = 0; i < tmp_weights.size(); i++) {
                    dfe_dump_stream_ << "Tap_" << i + 1 << ", ";
                }
                dfe_dump_stream_ << "\n";
                for (auto weight : tmp_weights) {
                    dfe_dump_stream_ << weight << ", ";
                }
                dfe_dump_stream_ << "\n";
            }
            std::vector<double> dummy_sig(test_sig);
            std::ostringstream params;
            std::vector<double> dfe_taps = dfe_->get_weights();
            params << "(example_rx";
            params << " (dfe_tap1 " << dfe_taps[0] << ")";
            params << " (dfe_tap2 " << dfe_taps[1] << ")";
            params << " (dfe_tap3 " << dfe_taps[2] << ")";
            params << " (dfe_tap4 " << dfe_taps[3] << ")";
            params << " (dfe_tap5 " << dfe_taps[4] << "))\n";
            bool adapted;
            while (true) {
                dummy_sig = test_sig;
                adapted = dfe_->apply(dummy_sig.data(), num_whole_bits * samples_per_bit_, dummy_clks.data());
                std::vector<double> dfe_taps = dfe_->get_weights();
                params << "(example_rx";
                params << " (dfe_tap1 " << dfe_taps[0] << ")";
                params << " (dfe_tap2 " << dfe_taps[1] << ")";
                params << " (dfe_tap3 " << dfe_taps[2] << ")";
                params << " (dfe_tap4 " << dfe_taps[3] << ")";
                params << " (dfe_tap5 " << dfe_taps[4] << "))\n";
                if (dump_dfe_adaptation_) {
                    std::vector<double> tmp_weights = dfe_->get_weights();
                    for (auto weight : tmp_weights) {
                        dfe_dump_stream_ << weight << ", ";
                    }
                    dfe_dump_stream_ << "\n";
                }
                bits += bits_per_call;
                if (bits >= MAX_INIT_ADAPT_BITS)
                    break;
            }
            param_str_ = params.str();
            if (!adapted)
                msg_ += "WARN: AmiRx::proc_imp(): DFE adaptation did not converge in the allowed time!\n";
        }
    }

    // Apply our filter.
    if (ctle_)
        ctle_->apply(impulse_matrix_, number_of_rows_);
    if (sig_tap_ == 2) // Return post-CTLE signal.
        return;
    if (dfe_) {
        std::vector<double> tmp_weights {1.0};
        for (auto weight : dfe_->get_weights())
            tmp_weights.push_back(-weight);
        FIRFilter tmp_filt(tmp_weights, samples_per_bit_);
        tmp_filt.apply(impulse_matrix_, number_of_rows_);
    }

    msg_ += debug_stream.str();
    } catch (...) {
        msg_ += debug_stream.str();
        std::cerr << debug_stream.str();
        std::cerr.flush();
//        throw;
        return;
    }
}

