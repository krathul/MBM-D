cmake_minimum_required(VERSION 2.8)
project(MBM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
find_package(OpenMP REQUIRED)
include_directories( . )
file(GLOB SOURCES *.cpp)

add_executable(DynamicMBM ${SOURCES})
add_executable(StaticMBM ${SOURCES})
target_compile_definitions(StaticMBM PUBLIC STATIC_MODE=1)

if(OPENMP_FOUND)
  target_compile_options(DynamicMBM PUBLIC "-Wall" "-Wextra" "-Wconversion" "-pedantic" "-fopenmp" "-g" "-O0" "-std=c++2a")
  target_compile_options(StaticMBM PUBLIC "-Wall" "-Wextra" "-Wconversion" "-pedantic" "-fopenmp" "-g" "-O0" "-std=c++2a")
endif()

