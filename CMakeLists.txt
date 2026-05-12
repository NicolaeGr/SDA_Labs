cmake_minimum_required(VERSION 3.10)
project(labs_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

file(GLOB COMMON_SOURCES "common/*.c")
add_library(common_lib ${COMMON_SOURCES})

target_include_directories(common_lib PUBLIC ${CMAKE_SOURCE_DIR})

add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)