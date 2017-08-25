cmake_minimum_required(VERSION 2.8)

project(baton)


add_library(
    libbaton-static
    STATIC
    src/baton.c)

target_include_directories(
    libbaton-static
    PRIVATE
    include)

set_target_properties(
    libbaton-static
    PROPERTIES
    OUTPUT_NAME baton)


if( TESTS )
    add_subdirectory(tests)
endif()
