SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

INCLUDE(CMakeForceCompiler)

CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH /usr/lib/arm-none-eabi)

link_directories(/usr/lib/arm-none-eabi/newlib/thumb)
include_directories(/usr/include/newlib/c++/4.9/arm-none-eabi/thumb)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(OBJCOPY_EXE arm-none-eabi-objcopy)

# chip specific
set(COMMON_EMBED_OPTIONS "-march=armv6-m -mtune=cortex-m0plus -mthumb -mfloat-abi=soft -T ${CMAKE_CURRENT_SOURCE_DIR}/chips/cortexm/memory_map.ld")

add_definitions(-D__MKL26Z64__)
include_directories(chips/cortexm/kl26z)
SET(NOS_CHIP_FILES
        chips/cortexm/kl26z/core.c
        chips/cortexm/kl26z/startup.c)