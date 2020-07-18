/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2020 Bolder Flight Systems
*/

#ifndef INCLUDE_FILTER_DIGITAL_FILTER_1D_H_
#define INCLUDE_FILTER_DIGITAL_FILTER_1D_H_

#include <cstdint>
#include <array>
#include <algorithm>

namespace filters {

template<typename T, std::size_t NUM_LEN, std::size_t DENOM_LEN>
class DigitalFilter1D {
 public:
  DigitalFilter1D(const std::array<T, NUM_LEN> &num_coeff, const std::array<T, DENOM_LEN> &denom_coeff) {
    num_ = num_coeff;
    denom_ = denom_coeff;
    /* Scale all coefficients by den[0] if available */
    if (denom_.size() > 0) {
      /* Prevent divide by zero */
      if (denom_[0] != 0) {
        for (std::size_t i = 0; i < num_.size(); i++) {
          num_[i] = num_[i] / denom_[0];
        }
        for (std::size_t i = 1; i < denom_.size(); i++) {
          denom_[i] = denom_[i] / denom_[0];
        }
      }
    }
  }
  T Filter(T input) {
    /* Shift all x and y values to the right 1 */
    if (x_.size() > 0) {
      std::rotate(x_.data(), x_.data() + x_.size() - 1, x_.data() + x_.size());
    }
    if (y_.size() > 0) {
      std::rotate(y_.data(), y_.data() + y_.size() - 1, y_.data() + y_.size());
    }
    /* Grab the newest x value */
    x_[0] = input;
    /* Apply all num coefficients */
    feed_forward_ = 0;
    for (std::size_t i = 0; i < num_.size(); i++) {
      feed_forward_ += num_[i] * x_[i];
    }
    /* Apply all den coefficients */
    feed_back_ = 0;
    for (std::size_t i = 1; i < denom_.size(); i++) {
      feed_back_ += denom_[i] * y_[i];
    }
    /* Get the output */
    output_ = feed_forward_ - feed_back_;
    /* Grab the newest y value */
    if (y_.size() > 0) {
      y_[0] = output_;
    }
    return output_;
  }

 private:
  /* Filter coefficients and states */
  std::array<T, NUM_LEN> num_, x_;
  std::array<T, DENOM_LEN> denom_, y_;
  T feed_forward_ = 0;
  T feed_back_ = 0;
  T output_;
};

};  // namespace filters

#endif  // INCLUDE_FILTER_DIGITAL_FILTER_1D_H_
