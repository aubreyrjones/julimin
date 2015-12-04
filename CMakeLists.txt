cmake_minimum_required(VERSION 2.8.12)
project(julimin)

add_definitions(-D__MKL26Z64__)

set(COMMON_OPTIONS "-Os -fwhole-program -flto -Wall -Wno-unknown-pragmas -nostartfiles -ffast-math")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 ${COMMON_OPTIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${COMMON_OPTIONS}")

if (NOT UNIX)
    set(COMMON_EMBED_OPTIONS "-march=armv6-m -mtune=cortex-m0plus -mthumb -mfloat-abi=soft -T ${CMAKE_CURRENT_SOURCE_DIR}/kl26z.ld")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_EMBED_OPTIONS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_EMBED_OPTIONS}")
endif()

set(NOS_SOURCE_FILES
        nosem/fixed_func.cpp
        nosem/syscalls.c)

include_directories(nosem)

set(SOURCE_FILES main.cpp ${NOS_SOURCE_FILES})
add_executable(julimin ${SOURCE_FILES})
target_link_libraries(julimin)