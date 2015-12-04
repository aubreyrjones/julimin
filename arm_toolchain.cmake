SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

INCLUDE(CMakeForceCompiler)

CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH /usr/lib/arm-none-eabi)
link_directories(/usr/lib/arm-none-eabi/newlib)
include_directories(/usr/include/newlib /usr/include/newlib/c++/4.9 /usr/include/newlib/c++/4.9/arm-none-eabi/armv6-m)

# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
