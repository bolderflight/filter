[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![Bolder Flight Systems Logo](img/logo-words_75.png) &nbsp; &nbsp; ![Arduino Logo](img/arduino_logo_75.png)

# Filter
This is a library of digital filters. This library is compatible with Arduino ARM and with CMake build systems. It would also be easy to include with other projects, since it is a header only library.
   * [License](LICENSE.md)
   * [Changelog](CHANGELOG.md)
   * [Contributing guide](CONTRIBUTING.md)

# Installation

## Arduino
Use the Arduino Library Manager to install this library or clone to your Arduino/libraries folder. This library is added as:

```C++
#include "filter.h"
```

An example Arduino executable is located at *examples/arduino/filter_example/filter_example.ino*. Teensy 3.x, 4.x, and LC devices are used for testing under Arduino and this library should be compatible with other ARM devices. This library is *not* expected to work on AVR devices.

## CMake
CMake is used to build this library, which is exported as a library target called *filter*. The header is added as:

```C++
#include "filter.h"
```

The library can be also be compiled stand-alone using the CMake idiom of creating a *build* directory and then, from within that directory issuing:

```
cmake ..
make
```

This will build the library, an example executable called *filter_example*, and an executable for testing using the Google Test framework, called *filter_test*. The example executable source files are located at *examples/cmake/filter_example.cc*.

# Namespace
This library is within the namespace *bfs*

# Classes

## Filter

### Description

```
Filter dlpf(b, a)
y = dlpf(x) 
```

Filters the input data x using a rational transfer function defined by the numerator and denominator coefficients b and a, similar to MATLAB's *filter* function. If a[0] is not equal to 1, then filter normalizes the filter coefficients by a[0]. Therefore, a[0] must be nonzero.

### Methods

**Filter(const std::array<T, NUM_LEN> &num_coeff, const std::array<T, DENOM_LEN> &denom_coeff)** This creates a *Filter* object. It is a templated class and the constructor takes an array of the numerator coefficients and an array of the denomenator coefficients.

```C++
/* Moving average filter with a window size of 5 */
std::array<float, 5> b = {0.2, 0.2, 0.2, 0.2, 0.2};
std::array<float, 1> a = {1};
bfs::Filter<float, 5, 1> dlpf(b, a);
```

**T Update(T val)** Filters the input data returning the filtered value.

```C++
/* Filter data array x */
for (std::size_t i = 0; i < x.size(); i++) {
  y[i] = dlpf.Update(x[i]);
}
```

**void Reset()** Resets the filter states.

```C++
dlpf.Reset();
```

## Iir

This class implements a 1st order IIR filter given a desired cutoff and sampling frequency. Optionally, an initial value may also be passed for non-zero initial values.

### Methods

**void Init(T cutoff_hz, T samp_hz)** Initializes the IIR filter given a cutoff frequency and sampling rate.

```C++
/*
* An IIR filter with a 10 Hz cutoff frequency
* and a 50 Hz sampling rate
*/
bfs::Iir<float> dlpf;
dlpf.Init(10.0f, 50.0f);
```

**void Init(T cutoff_hz, T samp_hz, T initial_val)** Initializes the IIR filter given a cutoff frequency, sampling rate, and initial value.

```C++
/*
* An IIR filter with a 10 Hz cutoff frequency,
* a 50 Hz sampling rate, and an initial value
* of 101325.
*/
bfs::Iir<float> dlpf;
dlpf.Init(10.0f, 50.0f, 101325.0f);
```

**T Filter(T val)** Passes a new value to the filter and returns the filtered result.

```C++
dlpf.Filter(97600.0f);
```

## LPF2P

This class implements a 2nd order low pass filter given a desired cutoff and sampling frequency. Optionally, an initial value may also be passed for non-zero initial values.

### Methods

**void Init(T cutoff_hz, T samp_hz)** Initializes the 2nd order LP filter given a cutoff frequency and sampling rate.

```C++
/*
* A 2nd order LP filter with a 10 Hz cutoff frequency
* and a 50 Hz sampling rate
*/
bfs::Lpf2p<float> dlpf;
dlpf.Init(10.0f, 50.0f);
```

**void Init(T cutoff_hz, T samp_hz, T initial_val)** Initializes the 2nd order LP filter given a cutoff frequency, sampling rate, and initial value.

```C++
/*
* A 2nd order LP filter with a 10 Hz cutoff frequency,
* a 50 Hz sampling rate, and an initial value
* of 101325.
*/
bfs::Lpf2p<float> dlpf;
dlpf.Init(10.0f, 50.0f, 101325.0f);
```

**T Filter(T val)** Passes a new value to the filter and returns the filtered result.

```C++
dlpf.Filter(97600.0f);
```