cmake_minimum_required(VERSION 3.10)
project(lab1 C)

file(GLOB_RECURSE LAB_SOURCES CONFIGURE_DEPENDS *.c)

add_executable(lab1 ${LAB_SOURCES})

target_link_libraries(lab1 PRIVATE common_lib m)
