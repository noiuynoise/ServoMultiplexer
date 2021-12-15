cmake_minimum_required(VERSION 3.0)



set(ARM_GCC_BIN "/usr/share/gcc-arm-none-eabi-10.3-2021.10/bin")
set(STM32_CUBE "/home/andrew/stm32_stuff/STM32CubeF4")

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/arm-gcc-toolchain.cmake")
project(stm32 LANGUAGES C ASM)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c")

set(LINKER_SCRIPT "STM32F401CCUX_FLASH.ld")

set(WARNING_FLAGS "${WARNING_FLAGS} -Wall")
set(SHARED_FLAGS "${SHARED_FLAGS} -g -Os -flto -nostartfiles")
set(SHARED_FLAGS "${SHARED_FLAGS} -MMD -MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SHARED_FLAGS} ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SHARED_FLAGS} ${WARNING_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -flto -T ${LINKER_SCRIPT} -specs=nano.specs -specs=nosys.specs")

include_directories(
    "${STM32_CUBE}/Drivers/CMSIS/Device/ST/STM32F4xx/Include"
    "${STM32_CUBE}/Drivers/CMSIS/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
)

add_executable(stm32
    "${CMAKE_CURRENT_SOURCE_DIR}/Startup/startup_stm32f401ccux.s"
    ${SOURCES}
)

add_custom_command(
    TARGET stm32 POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} stm32 -O binary stm32.bin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Convert ELF to binary"
)