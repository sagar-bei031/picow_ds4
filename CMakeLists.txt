cmake_minimum_required(VERSION 3.13)

# Set the Pico board and SDK path
set(PICO_BOARD pico_w)
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})

# Set the path to BTSTACK
set(PICO_BTSTACK_PATH ${CMAKE_CURRENT_LIST_DIR}/btstack)

# Import the Pico SDK
include(pico_sdk_import.cmake)

# Project configuration
project(bt_hid C CXX ASM)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Include directories for your project
include_directories(inc)

# Add subdirectory for source files
add_subdirectory(src)
