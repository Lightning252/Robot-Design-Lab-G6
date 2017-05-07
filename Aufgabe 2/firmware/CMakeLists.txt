project(STM32)
cmake_minimum_required(VERSION 2.6)

#needs to be set from outside to work
#set(CMAKE_TOOLCHAIN_FILE  CodeSourcery.cmake)

set(COMPILE_DEFINITIONS_DEBUG    -O3 -ggdb)
set(COMPILE_DEFINITIONS_RELEASE  -O3)

#macro for stripping executables to flashable image
macro(OBJCOPY_FILE EXE_NAME)
 set(FO ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.bin)
 set(FI ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME})
 message(STATUS ${FO})
 add_custom_command(
  OUTPUT "${FO}"
  COMMAND ${CMAKE_OBJCOPY}
  ARGS -O binary ${FI} ${FO}
  DEPENDS ${FI})
 get_filename_component(TGT "${EXE_NAME}" NAME)
 add_custom_target("TargetObjCopy_${TGT}" ALL DEPENDS ${FO} VERBATIM)
 get_directory_property(extra_clean_files ADDITIONAL_MAKE_CLEAN_FILES)
 set_directory_properties(
  PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES "${extra_clean_files};${FO}")
 set_source_files_properties("${FO}" PROPERTIES GENERATED TRUE)
endmacro(OBJCOPY_FILE)

#compile asm files with gcc
set_source_files_properties(lib/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.s PROPERTIES LANGUAGE C)
set_source_files_properties(core_cm3.c PROPERTIES LANGUAGE C)

set(CMAKE_EXE_LINKER_FLAGS "-T \"${STM32_SOURCE_DIR}/stm_h103_rom.ld\" -nostartfiles -mcpu=cortex-m3 -mthumb")

add_definitions(
  -D STM32F10X_MD -D USE_STDPERIPH_DRIVER -D HSE_VALUE=16000000 -D USE_FULL_ASSERT
  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -mcpu=cortex-m3 -mthumb -fno-unwind-tables -fno-common -O3 -ggdb"
  )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -mcpu=cortex-m3 -mthumb -fno-unwind-tables -fno-common -O3 -ggdb"
  )

include_directories(
  common
  lib/STM32F10x_StdPeriph_Driver/inc
  lib/STM32F10x_StdPeriph_Driver
  lib/CMSIS/CM3/DeviceSupport/ST/STM32F10x
  lib/CMSIS/CM3/CoreSupport
  lib/
  )

add_library(stm32fw
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c
  lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c
  lib/STM32F10x_StdPeriph_Driver/src/misc.c
  lib/CMSIS/CM3/CoreSupport/core_cm3.c
  lib/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.s
)

add_library(stm32drivers
    drivers/assert.c
    drivers/i2c.c
    drivers/printf.c
    drivers/usart.c
    common/stm32f10x_it.c
    common/system_stm32f10x.c
)

target_link_libraries(stm32drivers stm32fw)

add_executable(main
    main.cpp
    led.cpp
    engine.cpp
    protocol.cpp
	servo.cpp
    crc.c
    drivers/syscalls.c
    stm32_specific.c
)

target_link_libraries(main stm32fw stm32drivers stm32fw m gcc)
objcopy_file(main)

add_custom_target(flash openocd -f ${STM32_SOURCE_DIR}/openocd.cfg 
                  DEPENDS ${FO})
