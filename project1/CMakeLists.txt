cmake_minimum_required(VERSION 3.10)
project(project1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

get_filename_component(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)

include_directories(
    ${PROJECT_ROOT}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SORT_SOURCES
    sort/bubble_sort.c
    sort/heap_sort.c
    sort/counting_sort.c
    sort/radix_sort.c
    sort/comb_sort.c
    sort/merge_sort.c
    sort/quick_sort.c
    sort/shell_sort.c
    sort/selection_sort.c
    sort/insertion_sort.c
)

set(LIB_SOURCES
    ${PROJECT_ROOT}/lib/menu.c
    ${PROJECT_ROOT}/lib/utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/array_utils.c
)

add_executable(project1
    main.c
    ${SORT_SOURCES}
    ${LIB_SOURCES}
)

target_link_libraries(project1 m)
