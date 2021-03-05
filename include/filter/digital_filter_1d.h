/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2021 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#ifndef INCLUDE_FILTER_DIGITAL_FILTER_1D_H_
#define INCLUDE_FILTER_DIGITAL_FILTER_1D_H_

#include <cstdint>
#include <array>
#include <algorithm>

namespace bfs {

template<typename T, std::size_t NUM_LEN, std::size_t DENOM_LEN>
class DigitalFilter1D {
 public:
  DigitalFilter1D(const std::array<T, NUM_LEN> &num_coeff,
                  const std::array<T, DENOM_LEN> &denom_coeff) {
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
  T Filter(const T input) {
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
  std::array<T, NUM_LEN> num_, x_ = {0};
  std::array<T, DENOM_LEN> denom_, y_ = {0};
  T feed_forward_ = 0;
  T feed_back_ = 0;
  T output_;
};

};  // namespace bfs

#endif  // INCLUDE_FILTER_DIGITAL_FILTER_1D_H_
