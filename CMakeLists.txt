cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(AutoTurretPico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
INCLUDE_DIRECTORIES(../micro_ros_raspberrypi_pico_sdk/libmicroros/include)

link_directories(../micro_ros_raspberrypi_pico_sdk/libmicroros)
link_directories(../micro_ros_raspberrypi_pico_sdk/libmicroros/*)
add_executable(turret
    turretMainLoop.cpp
    turretMainLoop.hpp
)
target_link_libraries(turret
    pico_stdlib
    microros
)

target_include_directories(turret PRIVATE
    libmicroros/include
)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Configure Pico
pico_enable_stdio_usb(turret 1)
pico_enable_stdio_uart(turret 0)
add_compile_definitions(PICO_UART_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_DEFAULT_CRLF=0)

# Generate UF2
pico_add_extra_outputs(turret)
