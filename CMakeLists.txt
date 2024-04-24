cmake_minimum_required(VERSION 2.8)
project(MBM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
find_package(OpenMP REQUIRED)
include_directories( . )
file(GLOB SOURCES *.cpp)
add_executable(MBM ${SOURCES})
if(OPENMP_FOUND)
  target_compile_options(MBM PUBLIC "-Wall" "-Wextra" "-Wconversion" "-pedantic" "-fopenmp" "-g" "-O0" "-std=c++2a")
endif()

