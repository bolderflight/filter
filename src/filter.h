/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2022 Bolder Flight Systems Inc
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

#ifndef FILTER_SRC_FILTER_H_  // NOLINT
#define FILTER_SRC_FILTER_H_

#if defined(ARDUINO)
#include <Arduino.h>
#endif
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <array>
#include "iir.h"  // NOLINT
#include "lpf2p.h"  // NOLINT

namespace bfs {

template<typename T, std::size_t NUM_LEN, std::size_t DEN_LEN>
class Filter {
 public:
  static_assert(std::is_floating_point<T>::value,
                "Only floating point types supported");
  static_assert(NUM_LEN > 0,
                "Filter numerator length must be greater than 0");
  static_assert(DEN_LEN > 0,
                "Filter denomenator length must be greater than 0");
  Filter(const std::array<T, NUM_LEN> &b, const std::array<T, DEN_LEN> &a) {
    b_ = b;
    a_ = a;
    /* Scale all coefficients by a[0] */
    /* Prevent divide by 0 */
    if (a_[0] != 0) {
      for (std::size_t i = 0; i < b_.size(); i++) {
        b_[i] = b_[i] / a_[0];
      }
      for (std::size_t i = 1; i < a_.size(); i++) {
        a_[i] = a_[i] / a_[0];
      }
    }
  }
  T Update(const T x) {
    /* Shift all x and y values to the right 1 */
    if (x_.size() > 0) {
      std::rotate(x_.data(), x_.data() + x_.size() - 1, x_.data() + x_.size());
    }
    if (y_.size() > 0) {
      std::rotate(y_.data(), y_.data() + y_.size() - 1, y_.data() + y_.size());
    }
    /* Grab the newest x value */
    x_[0] = x;
    /* Apply all b coefficients */
    feed_forward_ = 0;
    for (std::size_t i = 0; i < b_.size(); i++) {
      feed_forward_ += b_[i] * x_[i];
    }
    /* Apply all a coefficients */
    feedback_ = 0;
    for (std::size_t i = 1; i < a_.size(); i++) {
      feedback_ += a_[i] * y_[i];
    }
    /* Get the output */
    output_ = feed_forward_ - feedback_;
    /* Grab the newest y value */
    if (y_.size() > 0) {
      y_[0] = output_;
    }
    return output_;
  }
  void Reset() {
    for (std::size_t i = 0; i < x_.size(); i++) {
      x_[i] = 0;
    }
    for (std::size_t i = 0; i < y_.size(); i++) {
      y_[i] = 0;
    }
    feed_forward_ = 0;
    feedback_ = 0;
  }

 private:
  /* Filter coefficients and states */
  std::array<T, NUM_LEN> b_, x_;
  std::array<T, DEN_LEN> a_, y_;
  T feed_forward_ = 0;
  T feedback_ = 0;
  T output_;
};

}  // namespace bfs

#endif  // FILTER_SRC_FILTER_H_ NOLINT
