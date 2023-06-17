/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2023 Bolder Flight Systems Inc
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
#else
#include <cstddef>
#include <cstdint>
#include <cstring>
#endif

#include "iir.h"  // NOLINT

namespace bfs {

template<typename T, size_t N, size_t M>
class Filter {
 public:
  Filter(const T (&b)[N], const T (&a)[M]) {
    for (size_t i = 0; i < N; i++) {
      b_[i] = b[i];
    }
    for (size_t i = 0; i < M; i++) {
      a_[i] = a[i];
    }
    /* Scale all coefficients by a[0] */
    /* Prevent divide by 0 */
    if (a_[0] != 0) {
      for (size_t i = 0; i < N; i++) {
        b_[i] = b_[i] / a_[0];
      }
      for (size_t i = 1; i < M; i++) {
        a_[i] = a_[i] / a_[0];
      }
    }
  }
  T Update(const T x) {
    /* Shift all x and y values to the right 1 */
    memmove(&x_[1], &x_[0], sizeof(x_) - sizeof(x_[0]));
    memmove(&y_[1], &y_[0], sizeof(y_) - sizeof(y_[0]));
    /* Grab the newest x value */
    x_[0] = x;
    /* Apply all b coefficients */
    feed_forward_ = 0;
    for (size_t i = 0; i < N; i++) {
      feed_forward_ += b_[i] * x_[i];
    }
    /* Apply all a coefficients */
    feedback_ = 0;
    for (size_t i = 1; i < M; i++) {
      feedback_ += a_[i] * y_[i];
    }
    /* Get the output */
    output_ = feed_forward_ - feedback_;
    /* Grab the newest y value */
    y_[0] = output_;
    return output_;
  }
  void Reset() {
    for (size_t i = 0; i < N; i++) {
      x_[i] = 0;
    }
    for (size_t i = 0; i < M; i++) {
      y_[i] = 0;
    }
    feed_forward_ = 0;
    feedback_ = 0;
  }

 private:
  /* Filter coefficients and states */
  T b_[N], x_[N];
  T a_[M], y_[M];
  T feed_forward_ = 0;
  T feedback_ = 0;
  T output_;
};

}  // namespace bfs

#endif  // FILTER_SRC_FILTER_H_ NOLINT
