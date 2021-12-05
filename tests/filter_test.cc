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

#include "filter.h"
#include "gtest/gtest.h"

TEST(Filter, MovingWindow) {
  /* Input values */
  std::array<float, 100> x = {
    0.2037,
    0.1630,
    -0.0948,
    0.0391,
    -0.0931,
    -0.2876,
    -0.3020,
    -0.2931,
    -0.2468,
    -0.2994,
    -0.5535,
    -0.4001,
    -0.4508,
    -0.6132,
    -0.5761,
    -0.7791,
    -0.7443,
    -0.6525,
    -0.7116,
    -0.6943,
    -0.7910,
    -0.9629,
    -0.7725,
    -0.7603,
    -0.8292,
    -0.8104,
    -0.8111,
    -0.8918,
    -0.8149,
    -0.9210,
    -0.7685,
    -0.9144,
    -0.8268,
    -0.8545,
    -0.8083,
    -0.5899,
    -0.5820,
    -0.6334,
    -0.4292,
    -0.6095,
    -0.4574,
    -0.4183,
    -0.2668,
    -0.2021,
    -0.2953,
    -0.1593,
    -0.1089,
    0.0036,
    0.0823,
    0.1569,
    0.1007,
    0.2650,
    0.3218,
    0.2610,
    0.3115,
    0.4666,
    0.6409,
    0.5433,
    0.6600,
    0.6230,
    0.8060,
    0.7305,
    0.8392,
    0.9305,
    1.0185,
    1.0724,
    1.0028,
    0.9306,
    0.9597,
    1.0094,
    1.1740,
    1.0424,
    1.1934,
    1.0577,
    1.2322,
    1.0864,
    1.0430,
    1.0476,
    1.1258,
    1.0732,
    1.0221,
    1.1173,
    1.0278,
    0.9872,
    1.0439,
    0.8476,
    0.9239,
    0.8785,
    0.7379,
    0.7349,
    0.5596,
    0.4997,
    0.5625,
    0.5665,
    0.5455,
    0.2836,
    0.3315,
    0.2439,
    0.0664,
    0.0843
  };
  /* Output from MATLAB */
  std::array<float, x.size()> z = {
      0.04074,
      0.07334,
      0.05438,
      0.0622,
      0.04358,
    -0.05468,
    -0.14768,
    -0.18734,
    -0.24452,
    -0.28578,
    -0.33896,
    -0.35858,
    -0.39012,
      -0.4634,
    -0.51874,
    -0.56386,
      -0.6327,
    -0.67304,
    -0.69272,
    -0.71636,
    -0.71874,
    -0.76246,
    -0.78646,
      -0.7962,
    -0.82318,
    -0.82706,
      -0.7967,
    -0.82056,
    -0.83148,
    -0.84984,
    -0.84146,
    -0.86212,
    -0.84912,
    -0.85704,
      -0.8345,
    -0.79878,
      -0.7323,
    -0.69362,
    -0.60856,
      -0.5688,
      -0.5423,
    -0.50956,
    -0.43624,
    -0.39082,
    -0.32798,
    -0.26836,
    -0.20648,
      -0.1524,
    -0.09552,
    -0.00508,
      0.04692,
      0.1217,
      0.18534,
      0.22108,
        0.252,
      0.32518,
      0.40036,
      0.44466,
      0.52446,
      0.58676,
      0.65464,
      0.67256,
      0.73174,
      0.78584,
      0.86494,
      0.91822,
      0.97268,
      0.99096,
      0.9968,
      0.99498,
      1.0153,
      1.02322,
      1.07578,
      1.09538,
      1.13994,
      1.12242,
      1.12254,
      1.09338,
        1.107,
      1.0752,
      1.06234,
      1.0772,
      1.07324,
      1.04552,
      1.03966,
      1.00476,
      0.96608,
      0.93622,
      0.88636,
      0.82456,
      0.76696,
      0.68212,
      0.61892,
      0.58464,
      0.54676,
      0.49156,
      0.45792,
      0.3942,
      0.29418,
      0.20194
  };
  /* Filter config */
  std::array<float, 5> b = {0.2, 0.2, 0.2, 0.2, 0.2};
  std::array<float, 1> a = {1};
  bfs::Filter<float, 5, 1> dlpf(b, a);
  /* Output from filter */
  std::array<float, x.size()> y;
  /* Test filter */
  for (std::size_t i = 0; i < x.size(); i++) {
    y[i] = dlpf.Update(x[i]);
    EXPECT_FLOAT_EQ(y[i], z[i]);
  }
  /* Reset filter */
  dlpf.Reset();
  /* Test again */
  for (std::size_t i = 0; i < x.size(); i++) {
    y[i] = dlpf.Update(x[i]);
    EXPECT_FLOAT_EQ(y[i], z[i]);
  }
}
