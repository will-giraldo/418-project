cmake_minimum_required(VERSION 3.1)

# project name
project(NaturalSelection)

# specify C++ standard
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)   
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set (SDL2_DIR "./SDL")

#include SDL
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# add other files
file(GLOB SOURCES
    ./*.h
    ./*.cpp
    ./utils/*.h
)

# add the executable
add_executable(NaturalSelection main.cpp)
target_link_libraries(NaturalSelection ${SDL2_LIBRARIES})