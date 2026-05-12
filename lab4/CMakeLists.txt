cmake_minimum_required(VERSION 3.10)
project(lab4 C)

file(GLOB_RECURSE LAB_SOURCES CONFIGURE_DEPENDS *.c)

add_executable(lab4 ${LAB_SOURCES})

target_link_libraries(lab4 PRIVATE common_lib m)
