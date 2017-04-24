# the name of the target operating system
#
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION   1)

# which compilers to use for C and C++
#
set(CMAKE_C_COMPILER       arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER     arm-none-eabi-g++)
set(CMAKE_OBJDUMP          arm-none-eabi-objdump)
#set(CMAKE_CXX_LINK_EXECUTABLE arm-none-linux-gnueabi-ld)
#set(CMAKE_C_LINK_EXECUTABLE   arm-none-linux-gnueabi-ld)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment,
# search programs in the host environment
#
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
