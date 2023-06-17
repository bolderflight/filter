# Changelog

## v4.0.0
- Updated filter to remove dependency on array and algorithm. Split IIR into header and source.

## v3.3.0
- Removed second order low pass filter - there are issues with initializing a value.

## v3.2.0
- Pulled in second order low pass filter from Tuan Luong

## v3.1.0
- Added IIR filter

## v3.0.1
- Updated Arduino example to not include iostream and to increase resolution displayed
- Removed initialization of x and y states in class, it wasn't needed and was causing issues with the Arduino compilation

## v3.0.0
- Only includes 1D digital filter, similar to MATLAB's filter function
- Updated directory structure to support Arduino in addition to CMake builds
- Added an Arduino example
- Updated README

## v2.1.1
- Fixing bug with IIR filter where template arguements weren't passed to BFS_2PI

## v2.1.0
- Added simple low-pass IIR filter

## v2.0.0
- Updated namespace to *bfs*

## v1.0.3
- Updated CONTRIBUTING
- Updated CI/CD pipeline to care about line length
- Updated digital_filter_1d.h to meet line length requirements

## v1.0.2
- Updated CONTRIBUTING

## v1.0.1
- Updated license to MIT

## v1.0.0
- Initial baseline
