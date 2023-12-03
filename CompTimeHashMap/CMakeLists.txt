cmake_minimum_required(VERSION 3.20)

project(TypeTraits LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++20)

add_executable(HashMap main.cpp constexpr_hashmap.hpp)