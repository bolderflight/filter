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

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <cmath>
#include <cstddef>
#include <cstdint>
#endif

#include "lpf2p.h"  // NOLINT
#include "units.h"  // NOLINT
#include <algorithm>


namespace bfs {
template<typename T> 
void Lpf2p<T>::Init(const T cutoff_hz, const T samp_hz) {
    cutoff_hz_ = std::min(cutoff_hz, samp_hz * static_cast<T>(0.4)); // Keep cutoff freq below Nyquist
    T fr = samp_hz / cutoff_hz_;
    T ohm = std::tan(static_cast<T>(BFS_PI_DOUBLE) / fr);
    T c = static_cast<T>(1) + static_cast<T>(2) * std::cos(static_cast<T>(BFS_PI_DOUBLE)/ static_cast<T>(4)) * ohm + ohm * ohm;
    b0_ = ohm * ohm / c;
    b1_ = static_cast<T>(2) * b0_;
    b2_ = b0_;
    
    a1_ = static_cast<T>(2) * (ohm * ohm - static_cast<T>(1)) / c;
    a2_ = (static_cast<T>(1) - static_cast<T>(2) * std::cos(static_cast<T>(BFS_PI_DOUBLE)/ static_cast<T>(4)) * ohm + ohm * ohm) / c;
    initialized_ = true;
}

template<typename T>
void Lpf2p<T>::Init(const T cutoff_hz, const T samp_hz, const T initial_val) {
    Init(cutoff_hz, samp_hz); //Calculate the necessary filter coefficients

    prev_output_ = delay1_ = delay2_ = initial_val * (static_cast<T>(1) / (static_cast<T>(1) + a1_ + a2_));
}

template<typename T>
T Lpf2p<T>::Filter(const T val, const T cutoff_hz, const T samp_hz) {
    if (!initialized_){
        Init(cutoff_hz, samp_hz, val);
    }

    T ret;
    prev_output_ = val - delay1_ * a1_ - delay2_ * a2_;
    ret = prev_output_ * b0_ + delay1_ * b1_ + delay2_ * b2_;
    delay2_ = delay1_;
    delay1_ = prev_output_;
    return ret;
}


}   //namespace bfs