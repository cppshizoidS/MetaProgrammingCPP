cmake_minimum_required(VERSION 3.26)
project(ch02)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS) #for clangd

add_compile_options(-Wall -std=c++23)

add_executable(ch02 source1.cpp source2.cpp wrapper.h main.cpp)
