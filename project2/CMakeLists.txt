cmake_minimum_required(VERSION 3.10)
project(project2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

get_filename_component(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)

include_directories(
    ${PROJECT_ROOT}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(LIB_SOURCES
    ${PROJECT_ROOT}/lib/menu.c
    ${PROJECT_ROOT}/lib/utils.c
)

add_executable(project2
    main.c
    ${LIB_SOURCES}
)

target_link_libraries(project2 m)
