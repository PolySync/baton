project(tests)


add_definitions(-DTESTS)


add_library(
    cli-shared
    SHARED
    ${CMAKE_SOURCE_DIR}/src/main.c)

target_link_libraries(
    cli-shared
    PRIVATE
    libbaton-shared)

target_include_directories(
    cli-shared
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include)

set_target_properties(
    cli-shared
    PROPERTIES
    OUTPUT_NAME cli)

add_custom_target(
    run-unit-tests
    DEPENDS
    run-cli-unit-tests
    run-lib-unit-tests)


# generate random port for test to avoid conflicts
string(
    RANDOM
    LENGTH 3
    ALPHABET "0123456789"
    PORT_SUFFIX)

set(
    CUCUMBER_PORT
    "25${PORT_SUFFIX}")

add_subdirectory(cli)
add_subdirectory(lib)
