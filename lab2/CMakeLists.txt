cmake_minimum_required(VERSION 3.10)
project(lab2 C)

file(GLOB_RECURSE LAB_SOURCES CONFIGURE_DEPENDS *.c)

add_executable(lab2 ${LAB_SOURCES})

target_link_libraries(lab2 PRIVATE common_lib m)
