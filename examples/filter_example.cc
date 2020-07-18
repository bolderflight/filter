/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2020 Bolder Flight Systems
*/

#include "filter/digital_filter_1d.h"
#include <array>

int main() {
  std::array<float, 2> num;
  std::array<float, 3> denom;
  filters::DigitalFilter1D dlpf(num, denom);
  
}
