cmake_minimum_required( VERSION 3.20 )

project( patter_match )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if( CMAKE_PROJECT_NAME STREQUAL "patter_match" )
    option( patter_match_TESTS "Build patter_match tests" ON )
    option( patter_match_DEMOS "Build patter_match demos" ON )
else()
    option( patter_match_TESTS "Build patter_match tests" OFF )
    option( patter_match_DEMOS "Build patter_match demos" OFF )
endif()

find_package( Boost )
find_package( Gtest QUIET )
find_package( Nonius QUIET )

add_library( patter_match INTERFACE )
target_compile_features( patter_match INTERFACE cxx_std_23 )
target_include_directories( patter_match INTERFACE include )

if( patter_match_TESTS )

    add_executable(  patter_match_polyobj demos/match_polyobj.cpp )
    target_link_libraries(  patter_match_polyobj PRIVATE patter_match )

    add_executable(  patter_match_any demos/match_any.cpp )
    set_target_properties(  patter_match_any PROPERTIES CXX_STANDARD 23 )
    target_link_libraries(  patter_match_any PRIVATE patter_match )

    add_executable(  customize_patter_match_for_variant demos/customize_match_for_variant.cpp )
    set_target_properties(  customize_patter_match_for_variant PROPERTIES CXX_STANDARD 23 )
    target_link_libraries(  customize_patter_match_for_variant PRIVATE patter_match )

    add_executable(  overload demos/overload.cpp )
    set_target_properties(  overload PROPERTIES CXX_STANDARD 23 )
    target_link_libraries(  overload PRIVATE patter_match )

endif()

if( patter_match_TESTS )

    set(files_test
        tests/main.cpp
        tests/match.test.cpp
        tests/match_any.test.cpp
        tests/meta.test.cpp
        tests/overload.test.cpp
        tests/signature.test.cpp
        tests/type_hierarchy.test.cpp
    )

    add_executable( patter_match_tests ${files_test} )
    set_target_properties( patter_match_tests PROPERTIES CXX_STANDARD 17 )
    target_link_libraries( patter_match_tests PRIVATE patter_match gtest )

endif()

if (Nonius)
    add_executable( benchmarks "benchmarks/dynamic_cast.cpp" )
    target_link_libraries( benchmarks LINK_PUBLIC patter_match nonius )
else()
    message("<> Nonius not found, won't build benchmarks.")
endif()
