cmake_minimum_required(VERSION 3.12)
set(PICO_SDK_PATH /home/cameron/pico-sdk)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(AutoTurretPico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico SDK
pico_sdk_init()

# Set paths and directories
set(LIBMICROROS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libmicroros/include)

# Create a custom target to generate libmicroros.a if needed
add_custom_target(libmicroros
    COMMAND ${CMAKE_AR} rcs libmicroros.a
    WORKING_DIRECTORY ${LIBMICROROS_INCLUDE_DIR}
)

# Include directories
include_directories(${LIBMICROROS_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

# Add customTime library
add_library(customTime
    clockFix.c
    clockFix.h
)

target_link_libraries(customTime 
    pico_stdlib
)

# Add customRMW library for RMW transport
add_library(customRMW    
    pico_uart_transport.c
)

target_link_libraries(customRMW 
    pico_stdlib
)

# Add executable target
add_executable(turret
    turretMainLoop.hpp
    turretMainLoop.cpp
)

target_link_libraries(turret
    pico_stdlib
    ${CMAKE_SOURCE_DIR}/libmicroros/libmicroros.a
    customTime
    customRMW
)

# Compiler flags
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Configure Pico settings
pico_enable_stdio_usb(turret 1)
pico_enable_stdio_uart(turret 0)
add_compile_definitions(PICO_UART_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_DEFAULT_CRLF=0)

# Generate UF2 file
pico_add_extra_outputs(turret)
