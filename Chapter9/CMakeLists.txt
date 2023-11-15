cmake_minimum_required(VERSION 3.26)
project(ch09)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd

add_compile_options(-Wall -std=c++23)

add_executable(ch09 main.cpp)
