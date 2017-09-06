cmake_minimum_required(VERSION 2.8)

project(baton)


set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra" )

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


add_library(
    libbaton-shared
    SHARED
    ${CMAKE_SOURCE_DIR}/src/baton.c)

target_include_directories(
    libbaton-shared
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include)

set_target_properties(
    libbaton-shared
    PROPERTIES
    OUTPUT_NAME baton)


add_executable(
    baton
    src/main.c)

target_link_libraries(
    baton
    PRIVATE
    libbaton-static)

target_include_directories(
    baton
    PRIVATE
    include
    src )


if(TESTS)
    add_subdirectory(tests)
endif()
