# Project and cmake setup
cmake_minimum_required (VERSION 2.8.11)
project (TestOffload)

# Allow CMake to find custom modules 
set (CMAKE_MODULE_PATH ../CMakeModules)
#set (ICC_COMP_FLAGS "-tbb")

# Run FindTBB.cmake module and include found directories
find_package(TBB REQUIRED)
include_directories(${TBB_INCLUDE_DIRS}) 
include_directories(../headers)
include_directories(../input)

# Set the cxx compiler to icc
#set (CMAKE_CXX_COMPILER /opt/intel/composer_xe_2015.1.133/bin/intel64/icc)

# Add a new target and set target flag as well as target libraries
add_executable (testProg test_mult_tbb_lambda.cpp)
target_link_libraries(testProg ${TBB_LIBRARIES})

if(CMAKE_VERSION VERSION_GREATER 3.1.0 OR CMAKE_VERSION VERSION_EQUAL 3.1.0)
	set_property(TARGET testProg PROPERTY CMAKE_CXX_STANDARD 11)
	set_property(TARGET testProg PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
	message("C++11 Standard Set")
else()
	#set C++11 flag
	set (ICC_COMP_FLAGS "${ICC_COMP_FLAGS} -std=c++0x")

	#add CXX11 check script
	include (${CMAKE_MODULE_PATH}/CheckCXX11Features.cmake)

	#use CXX11 script to check for lambda support
	set (NOTHASLAMBDA 1)
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
endif()

#set flags for compiler
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ICC_COMP_FLAGS}")

