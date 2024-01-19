cmake_minimum_required(VERSION 3.27)
project(meta)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(meta INTERFACE src/meta/static_vector.hpp src/meta/const_string.hpp src/meta/token_stream.hpp src/meta/meta.hpp src/meta/meta_rules.hpp src/meta/expressions.hpp src/meta/types.hpp)
target_include_directories(meta INTERFACE src/)

add_executable(meta-example main.cpp)
target_link_libraries(meta-example PUBLIC meta)
