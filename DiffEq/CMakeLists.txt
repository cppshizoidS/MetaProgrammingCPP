cmake_minimum_required(VERSION 3.27)
project(DiffEquation)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(DiffEquation
        diff_methods.hpp
        example.cpp)
