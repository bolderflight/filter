# filter
This is a library of digital filters.
   * [License](LICENSE.md)
   * [Changelog](CHANGELOG.md)
   * [Contributing guide](CONTRIBUTING.md)

## Installation
CMake is used to build this library, which is exported as a library target called *filter*. The header is added as:

```
#include "filter/filter.h"
```

The library can be also be compiled stand-alone using the CMake idiom of creating a *build* directory and then, from within that directory issuing:

```
cmake ..
make
```

This will build the library and an example executable called *filter_example*. The example executable source files are located at *examples/filter_example.cc*.

## Namespace
This library is within the namespace *bfs*

## Classes

### DigitalFilter1D

#### Description
```
DigitalFilter1D dlpf(b, a)
y = dlpf(x) 
```

Filters the input data x using a rational transfer function defined by the numerator and denominator coefficients b and a.

If a[0] is not equal to 1, then filter normalizes the filter coefficients by a[0]. Therefore, a[0] must be nonzero.

#### Methods

**DigitalFilter1D(const std::array<T, NUM_LEN> &num_coeff, const std::array<T, DENOM_LEN> &denom_coeff)** This creates a DigitalFilter1D object. It is a templated class and the constructor takes an array of the numerator coefficients and an array of the denomenator coefficients.

```C++
/* Moving average filter with a window size of 5 */
std::array<float, 5> b = {0.2, 0.2, 0.2, 0.2, 0.2};
std::array<float, 1> a = {1};
bfs::DigitalFilter1D<float, 5, 1> dlpf(b, a);
```

**T Filter(T val)** Filters the input data returning the filtered value.

```C++
/* Filter data array x */
for (std::size_t i = 0; i < x.size(); i++) {
  y[i] = dlpf.Filter(x[i]);
}
```
