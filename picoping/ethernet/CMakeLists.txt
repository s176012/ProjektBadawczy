cmake_minimum_required(VERSION 3.19)

include($ENV{ROOT_DIR}/inc/pico_sdk_import.cmake)
include($ENV{ROOT_DIR}/inc/pico_extras_import.cmake)
include($ENV{ROOT_DIR}/inc/pico_enc28j60_import.cmake)

include_directories($ENV{ROOT_DIR}/inc/pico-enc28j60)
include_directories($ENV{ROOT_DIR}/inc/pico-extras)
include_directories($ENV{ROOT_DIR}/inc/pico-sdk)

project(ethernet)

set(CMAKE_C_STANDARD 11)
set(CXX_STANDARD 17)

pico_sdk_init()

include_directories(.)

add_executable(ethernet main.cpp)
target_link_libraries(ethernet pico_stdlib lwip pico_enc28j60)
pico_add_extra_outputs(ethernet)