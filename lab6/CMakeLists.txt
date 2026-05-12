cmake_minimum_required(VERSION 3.10)
project(lab6 C)

file(GLOB_RECURSE LAB_SOURCES CONFIGURE_DEPENDS *.c)

add_executable(lab6 ${LAB_SOURCES})

target_link_libraries(lab6 PRIVATE common_lib m)
