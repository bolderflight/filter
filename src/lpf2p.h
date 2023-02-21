/****************************************************************************
 *
 *   Copyright (C) 2012-2021 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/*
* 
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

///Author: Leonard Hall <LeonardTHall@gmail.com>
///Adapted to BFS by Tuan Luong <tdluong@crimson.ua.edu>

#ifndef FILTER_SRC_LPF2P_H_  // NOLINT
#define FILTER_SRC_LPF2P_H_

#if defined(ARDUINO)
#include <Arduino.h>
#endif
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include "units.h"  // NOLINT

namespace bfs {

template<typename T>
class Lpf2p {
 public:
  static_assert(std::is_floating_point<T>::value,
                "Only floating point types supported");
  void Init(T cutoff_hz, T samp_hz) {
    T fc = std::max(cutoff_hz, samp_hz * 0.001f);
    T fr = samp_hz / fc;
    T ohm = std::tan(BFS_PI<T> / fr);
    T cosPio4 = std::cos(BFS_PI<T> / static_cast<T>(4));
    T c = static_cast<T>(1) + static_cast<T>(2) * cosPio4 * ohm + ohm * ohm;
    
    b0_ = ohm * ohm / c;
    b1_ = static_cast<T>(2) * b0_;
    b2_ = b0_;

    a1_ = static_cast<T>(2) * (ohm * ohm - static_cast<T>(1)) / c;
    a2_ = (static_cast<T>(1) - static_cast<T>(2) * cosPio4 * ohm + ohm * ohm) / c;
  }

  void Init(T cutoff_hz, T samp_hz, T initial_val) {
    T fc = std::max(cutoff_hz, samp_hz * 0.001f);
    T fr = samp_hz / fc;
    T ohm = std::tan(BFS_PI<T> / fr);
    T cosPio4 = std::cos(BFS_PI<T> / static_cast<T>(4));
    T c = static_cast<T>(1) + static_cast<T>(2) * cosPio4 * ohm + ohm * ohm;
    
    b0_ = ohm * ohm / c;
    b1_ = static_cast<T>(2) * b0_;
    b2_ = b0_;

    a1_ = static_cast<T>(2) * (ohm * ohm - static_cast<T>(1)) / c;
    a2_ = (static_cast<T>(1) - static_cast<T>(2) * cosPio4 * ohm + ohm * ohm) / c;
    delay1_ = initial_val;
    delay2_ = initial_val;
  }

  T Filter(T val) {
    T ret;
    delay0_ = val - delay1_ * a1_ - delay2_ * a2_;
    ret = delay0_ * b0_ + delay1_ * b1_ + delay2_ * b2_;
    delay2_ = delay1_;
    delay1_ = delay0_;
    return ret;
  }

 private:
  T a1_ = static_cast<T>(0);
  T a2_ = static_cast<T>(0);
  T b0_ = static_cast<T>(1);
  T b1_ = static_cast<T>(0);
  T b2_ = static_cast<T>(0);
  T delay0_ = static_cast<T>(0);
  T delay1_= static_cast<T>(0);
  T delay2_ = static_cast<T>(0);;
};

}  // namespace bfs

#endif  // FILTER_SRC_LPF2P_H_ NOLINT
