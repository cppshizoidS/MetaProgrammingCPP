cmake_minimum_required(VERSION 3.26)
project(fmap)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

add_executable(fmap fmap.hpp main.cpp)