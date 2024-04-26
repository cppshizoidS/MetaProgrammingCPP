cmake_minimum_required(VERSION 3.28)
project(DiffEquation)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DiffEquation diff_methods.hpp main.cpp)
