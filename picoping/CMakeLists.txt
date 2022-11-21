cmake_minimum_required(VERSION 3.19)

include(${CMAKE_CURRENT_SOURCE_DIR}/inc/pico_sdk_import.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/inc/pico_extras_import.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/inc/pico_enc28j60_import.cmake)

add_subdirectory($ENV{ROOT_DIR}/inc/pico-enc28j60)
add_subdirectory($ENV{ROOT_DIR}/inc/pico-extras)
add_subdirectory($ENV{ROOT_DIR}/inc/pico-sdk)

project(picoping)

set(CMAKE_C_STANDARD 11)
set(CXX_STANDARD 17)

pico_sdk_init()

include_directories(.)

add_executable(picoping main.cpp)
target_link_libraries(picoping pico_stdlib lwip pico_enc28j60)
pico_add_extra_outputs(picoping)