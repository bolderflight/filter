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

#ifndef INCLUDE_FILTER_IIR_H_
#define INCLUDE_FILTER_IIR_H_

#include "units/units.h"

namespace bfs {

template<typename T>
class Iir {
 public:
  static_assert(std::is_floating_point<T>::value,
                "Only floating point types supported");
  void Init(T cutoff_hz, T samp_hz) {
    T fc = cutoff_hz / samp_hz;
    b_ = static_cast<T>(2) - std::cos(BFS_2PI<T> * fc) -
          std::sqrt(std::pow(static_cast<T>(2) - std::cos(BFS_2PI<T> * fc),
          static_cast<T>(2)) - static_cast<T>(1));
    a_ = static_cast<T>(1) - b_;
  }
  void Init(T cutoff_hz, T samp_hz, T initial_val) {
    T fc = cutoff_hz / samp_hz;
    b_ = static_cast<T>(2) - std::cos(BFS_2PI<T> * fc) -
          std::sqrt(std::pow(static_cast<T>(2) - std::cos(BFS_2PI<T> * fc),
          static_cast<T>(2)) - static_cast<T>(1));
    a_ = static_cast<T>(1) - b_;
    prev_output_ = initial_val;
  }
  T Filter(T val) {
    T ret;
    ret = a_ * val + b_ * prev_output_;
    prev_output_ = ret;
    return ret;
  }

 private:
  T a_, b_;
  T prev_output_ = static_cast<T>(0);
};

}  // namespace bfs

#endif  // INCLUDE_FILTER_IIR_H_
