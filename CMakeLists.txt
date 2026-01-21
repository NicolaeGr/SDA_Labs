cmake_minimum_required(VERSION 3.10)
project(labs_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_subdirectory(project1)
add_subdirectory(project2)
# add_subdirectory(project3)
# add_subdirectory(project4)
# add_subdirectory(project5)
# add_subdirectory(project6)