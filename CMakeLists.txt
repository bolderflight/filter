cmake_minimum_required(VERSION 3.14)
# Project information
project(Filter
  VERSION 3.2.0
  DESCRIPTION "Digital filter library."
  LANGUAGES CXX
)
include(FetchContent)
FetchContent_Declare(
  units
  GIT_REPOSITORY 	https://github.com/bolderflight/units.git
  GIT_TAG v4.1.0
)
FetchContent_MakeAvailable(units)
# Add the library target
add_library(filter INTERFACE)
# Link libraries
target_link_libraries(filter INTERFACE units)
# Setup include directories 
target_include_directories(filter INTERFACE src/)

# Example and unit testing if this project is built separately
if (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Add the example target
  add_executable(filter_example examples/cmake/filter_example.cc)
  # Add the includes
  target_include_directories(filter_example PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  # Link libraries to the test target
  target_link_libraries(filter_example
    PRIVATE 
      filter
  )
  # Fetch google test
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )
  FetchContent_MakeAvailable(googletest)
  enable_testing()
  include(GoogleTest)
  # Add the test target
  add_executable(filter_test tests/filter_test.cc)
  # Add the includes
  target_include_directories(filter_test PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  # Link libraries to the test target
  target_link_libraries(filter_test
    PRIVATE 
      filter
      gtest_main 
      gtest
      gmock
  )
  # Discover unit tests
  gtest_discover_tests(filter_test)
endif()
