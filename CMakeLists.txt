cmake_minimum_required(VERSION 2.8.12)
project(julimin)

set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

set(MAX_OPTIMIZE "-Os -fwhole-program -flto")
set(COMMON_OPTIONS "-Wall -Wno-unknown-pragmas -ffunction-sections -fdata-sections -specs=nano.specs -finline-functions -nostdlib -nostartfiles -ffast-math ${MAX_OPTIMIZE} -Wl,--gc-sections")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 ${COMMON_OPTIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-exceptions -fno-rtti ${COMMON_OPTIONS}")

include_directories(/usr/include/newlib /usr/include/newlib/c++/4.9)

if (NOT UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_EMBED_OPTIONS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_EMBED_OPTIONS}")
endif()

# ============ NOS ===============

set(NOS_SOURCE_FILES
        ${NOS_CHIP_FILES}
        nos/math/fixed_impl/fixed_func.cpp
        nos/core/syscalls.cpp
        nos/core/delay.cpp
        nos/time/SystemTimer.cpp
        nos/analog/dac.h nos/comm/i2c.cpp nos/comm/i2c.h nos/comm/uart.cpp nos/comm/uart.h nos/core/sync.h nos/comm/pin.cpp nos/comm/pin.h)

include_directories(nos)

# ================================

set(SOURCE_FILES main.cpp ${KSDK_SOURCE_FILES} ${NOS_SOURCE_FILES} nos/core/errors.h tables/voices.h tables/voices.cpp)
add_executable(julimin ${SOURCE_FILES})
target_link_libraries(julimin stdc++ c m gcc)

if (NOT UNIX)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/out.hex"
            COMMAND ${OBJCOPY_EXE} ARGS "-O" ihex julimin out.hex
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            DEPENDS julimin)
    add_custom_target(hexout ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/out.hex")

    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/image.bin"
            COMMAND ${OBJCOPY_EXE} ARGS "-O" binary julimin image.bin
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            DEPENDS julimin)
    add_custom_target(binout ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/image.bin")

    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/.hex_uploaded"
            COMMAND ${UPLOAD_HEX_EXE} ARGS "out.hex"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/out.hex")
    add_custom_target(upload DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/.hex_uploaded")
endif()
