cmake_minimum_required(VERSION 3.10)
project(lab3 C)

file(GLOB_RECURSE LAB_SOURCES CONFIGURE_DEPENDS *.c)

add_executable(lab3 ${LAB_SOURCES})

target_link_libraries(lab3 PRIVATE common_lib m)
