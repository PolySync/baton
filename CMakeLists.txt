cmake_minimum_required(VERSION 2.8)

project(baton)

add_library(
    libbaton
    STATIC
    src/baton.c)

target_include_directories(
    libbaton
    PRIVATE
    include)

SET_TARGET_PROPERTIES(
    libbaton
    PROPERTIES
    OUTPUT_NAME baton)
