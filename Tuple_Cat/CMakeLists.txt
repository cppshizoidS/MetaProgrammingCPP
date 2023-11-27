cmake_minimum_required(VERSION 3.20)
project(tuple_cat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tuple_cat main.cpp Meta.hpp Tuple.hpp Utils.hpp)