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

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <cmath>
#include <cstddef>
#include <cstdint>
#endif

#include "iir.h"  // NOLINT
#include "units.h"  // NOLINT

namespace bfs {

void Iir::Init(const float cutoff_hz, const float samp_hz) {
  fc_ = cutoff_hz / samp_hz;
  b_ = 2.0f - cosf(BFS_2PI_FLOAT * fc_) -
        sqrtf(powf(2.0f - cosf(BFS_2PI_FLOAT * fc_), 2.0f) - 1.0f);
  a_ = 1.0f - b_;
}

void Iir::Init(const float cutoff_hz, const float samp_hz,
               const float initial_val) {
  float fc = cutoff_hz / samp_hz;
  b_ = 2.0f - cosf(BFS_2PI_FLOAT * fc) -
        sqrtf(powf(2.0f - cosf(BFS_2PI_FLOAT * fc), 2.0f) - 1.0f);
  a_ = 1.0f - b_;
  prev_output_ = initial_val;
}

float Iir::Filter(const float val) {
  ret_ = a_ * val + b_ * prev_output_;
  prev_output_ = ret_;
  return ret_;
}

}  // namespace bfs
