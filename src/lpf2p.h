/*
* Tuan D Luong
* tdluong@crimson.ua.edu
* 
*
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

#ifndef FILTER_SRC_LPF2P_H_  // NOLINT
#define FILTER_SRC_LPF2P_H_

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <cmath>
#include <cstddef>
#include <cstdint>
#endif

namespace bfs {

template<typename T>
class Lpf2p {
 public:
  void Init(const T cutoff_hz, const T samp_hz);
  void Init(const T cutoff_hz, const T samp_hz,
            const T initial_val);
  T Filter(const T val, const T cutoff_hz=static_cast<T>(0), 
            const T samp_hz=static_cast<T>(0));

 private:
  bool initialized_ = false;
  T a1_, a2_, b0_, b1_, b2_;
  T cutoff_hz_, ret_val_;
  T prev_output_ = static_cast<T>(0);
  T delay1_ = prev_output_;
  T delay2_ = prev_output_;
};

}  // namespace bfs

#endif  // FILTER_SRC_LPF2P_H_ NOLINT