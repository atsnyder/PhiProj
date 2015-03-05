# Project and cmake setup
cmake_minimum_required (VERSION 2.8.11)
project (TestOffload)

# Allow CMake to find custom modules 
set (CMAKE_MODULE_PATH ../CMakeModules)
set (ICC_COMP_FLAGS "-tbb -std=c++0x")
set (NOTHASLAMBDA 1)

# Run FindTBB.cmake module and include found directories
find_package(TBB REQUIRED)
include_directories(${TBB_INCLUDE_DIRS}) 
include_directories(../headers)
include_directories(../input)

# Find thread library
#find_library(
#	THREAD_LIB
#	libpthread.so.0
#	HINTS /usr/linux-k1om-4.7/linux-k1om/lib64
#	)

# Set the cxx compiler to icc
set (CMAKE_CXX_COMPILER /opt/intel/composer_xe_2015.1.133/bin/intel64/icc)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ICC_COMP_FLAGS}")
include (../CMakeModules/CheckCXX11Features.cmake)

# Add a new target and set target flag as well as target libraries
add_executable (testProg test_mult_tbb_lambda.cpp)
target_link_libraries(testProg ${TBB_LIBRARIES})
foreach(flag ${CXX11_FEATURE_LIST})
	if(${flag} STREQUAL HAS_CXX11_LAMBDA)
		set(NOTHASLAMBDA 0)
	endif()
endforeach()
if(NOTHASLAMBDA)
	message (FATAL_ERROR "Compiler does not support C++11 lambda functions.")
else()
	message ("C++11 lambda functions supported.")
endif()

