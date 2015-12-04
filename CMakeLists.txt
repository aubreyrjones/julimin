cmake_minimum_required(VERSION 2.8.12)
project(julimin)

set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

set(COMMON_OPTIONS "-Os -s -fwhole-program -flto -nostdlib -Wall -Wno-unknown-pragmas -nostartfiles -ffast-math -Wl,--gc-sections")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 ${COMMON_OPTIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti ${COMMON_OPTIONS}")

include_directories(/usr/include/newlib /usr/include/newlib/c++/4.9)

if (NOT UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_EMBED_OPTIONS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_EMBED_OPTIONS}")
endif()

set(NOS_SOURCE_FILES
        ${NOS_CHIP_FILES}
        nos/math/fixed_func.cpp
        nos/core/syscalls.c
        )

include_directories(nos)

set(SOURCE_FILES main.cpp ${NOS_SOURCE_FILES})
add_executable(julimin ${SOURCE_FILES})
target_link_libraries(julimin stdc++ c m gcc)

if (NOT UNIX)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/out.hex"
            COMMAND ${OBJCOPY_EXE} ARGS "-O" ihex julimin out.hex
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            DEPENDS julimin)
    add_custom_target(hexout ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/out.hex")
endif()