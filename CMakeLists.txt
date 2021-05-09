cmake_minimum_required(VERSION 3.13)
# Project information
project(Filter
  VERSION 2.1.1
  DESCRIPTION "Digital filters."
  LANGUAGES CXX
)
include(FetchContent)
# Fetch units
FetchContent_Declare(
	units
	GIT_REPOSITORY https://github.com/bolderflight/units.git
	GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(units)
# Add the library target
add_library(filter INTERFACE)
# Link libraries
target_link_libraries(filter
	INTERFACE
		units
)
# Setup include directories 
target_include_directories(filter INTERFACE include/)

# Example and unit testing if this project is built separately
if (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
	# Add the example target
	add_executable(filter_example examples/filter_example.cc)
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
endif()
