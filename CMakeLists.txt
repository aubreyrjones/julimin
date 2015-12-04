cmake_minimum_required(VERSION 2.8.12)
project(julimin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Os -ffast-math ")

if (Generic)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv6-m -mtune=cortex-m0plus -mthumb -mfloat-abi=soft")
endif()

set(NOS_SOURCE_FILES
        nosem/fixed_func.cpp
        nosem/syscalls.c)

include_directories(nosem)

set(SOURCE_FILES main.cpp ${NOS_SOURCE_FILES})
add_executable(julimin ${SOURCE_FILES})
target_link_libraries(julimin gcc)