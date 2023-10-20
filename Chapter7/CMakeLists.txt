cmake_minimum_required(VERSION 3.26)
project(ch7)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -std=c++23)

add_executable(ch7 main.cpp)
