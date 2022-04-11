cmake_minimum_required(VERSION 2.8)

# project name
project(NaturalSelection)

# specify C++ standard
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)   
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# add the executable
add_executable(NaturalSelection main.cpp)