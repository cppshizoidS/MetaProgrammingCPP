cmake_minimum_required(VERSION 3.26)
project(VectorAbs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(VectorAbs vec_abs.hpp main.cpp)
