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
set(UPLOAD_HEX_EXE "${CMAKE_CURRENT_SOURCE_DIR}/load-tlc.sh")

include_directories(
        "${KSDK}/platform/devices"
        "${KSDK}/platform/CMSIS/Include"
        "${KSDK}/platform/system/inc"
        "${KSDK}/platform/drivers/inc"
        "${KSDK}/platform/hal/inc"
        "${KSDK}/platform/osa/inc"
        "${KSDK}/platform/utilities/inc")

SET(KSDK_SOURCE_FILES
        "${KSDK}/platform/drivers/src/i2c/fsl_i2c_common.c"
        "${KSDK}/platform/drivers/src/i2c/fsl_i2c_irq.c"
        "${KSDK}/platform/drivers/src/i2c/fsl_i2c_lpm_callback.c"
        "${KSDK}/platform/drivers/src/i2c/fsl_i2c_master_driver.c"
        "${KSDK}/platform/drivers/src/i2c/fsl_i2c_shared_function.c"
        "${KSDK}/platform/hal/src/i2c/fsl_i2c_hal.c"
        "${KSDK}/platform/system/src/clock/MKL26Z4/fsl_clock_MKL26Z4.c"
        "${KSDK}/platform/system/src/hwtimer/fsl_hwtimer.c"
        "${KSDK}/platform/system/src/hwtimer/fsl_hwtimer_pit.c"
        "${KSDK}/platform/system/src/hwtimer/fsl_hwtimer_pit_irq.c"
        "${KSDK}/platform/system/src/hwtimer/fsl_hwtimer_systick.c"
        "${KSDK}/platform/system/src/hwtimer/fsl_hwtimer_systick_irq.c"
        "${KSDK}/platform/system/src/interrupt/fsl_interrupt_manager.c"
        "${KSDK}/platform/system/src/power/fsl_power_manager.c"
        "${KSDK}/platform/osa/src/fsl_os_abstraction_bm.c"
        )

# chip specific
add_definitions(-DCPU_MKL26Z64VFT4)
include_directories("${KSDK}/platform/devices/MKL26Z4/include" "${KSDK}/platform/devices/MKL26Z4/startup")
set(COMMON_EMBED_OPTIONS "-march=armv6-m -mtune=cortex-m0plus -mthumb -mfloat-abi=soft -T ${CMAKE_CURRENT_SOURCE_DIR}/chips/cortexm/memory_map.ld")

include_directories(chips/cortexm chips/cortexm/kl26z)
SET(NOS_CHIP_FILES
        chips/cortexm/kl26z/core.cpp
        chips/cortexm/kl26z/startup.c
        chips/cortexm/kl26z/chip_support.h)

