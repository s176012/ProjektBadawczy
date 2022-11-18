cmake_minimum_required(VERSION 3.19)

include(${CMAKE_CURRENT_SOURCE_DIR}/pico_sdk_import.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/pico_extras_import.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/pico_enc28j60_import.cmake)

project(picoping)

set(CMAKE_C_STANDARD 11)

pico_sdk_init()

include_directories(.)

add_executable(picoping main.c)
target_link_libraries(picoping pico_stdlib lwip pico_enc28j60)
pico_add_extra_outputs(picoping)