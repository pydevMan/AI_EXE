# Minimal version of CMake
cmake_minimum_required (VERSION 3.10)
 
# Build type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Debug' as none was specified.")
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()
 
# Define project name
project (CL_RUNNER)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
 
find_package( OpenCL REQUIRED )

include_directories( ${OPENCL_INCLUDE_DIR} )

# Source code of application		
set (code main.cc)

set (lib matrix/matrix.cc matrix/matrix.hh matrix/setup_opencl.cc matrix/setup_opencl.hh)
 
# Compiler flags
if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_CXX_FLAGS "-D_REETRANT -Wall -Wextra -pedantic -Wno-long-long")
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   	    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
	    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -O3 -fno-strict-aliasing")
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)
 

add_executable (CL_RUNNER ${code})
target_compile_options(CL_RUNNER -lOpenCL)