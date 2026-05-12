cmake_minimum_required(VERSION 3.10)
project(lab5 C)

file(GLOB_RECURSE LAB_SOURCES CONFIGURE_DEPENDS *.c)

add_executable(lab5 ${LAB_SOURCES})

target_link_libraries(lab5 PRIVATE common_lib m)
