cmake_minimum_required(VERSION 3.28)
project(Tensor)

set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD 23)

add_executable(${CMAKE_PROJECT_NAME} tensor.hpp main.cpp)