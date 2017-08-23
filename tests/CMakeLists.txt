project(unit-tests)


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
    unit-test
    ${CMAKE_CURRENT_SOURCE_DIR}/features/step_definitions/test.cpp)

target_link_libraries(
    unit-test
    PRIVATE
    libbaton-shared
    ${CMAKE_SOURCE_DIR}/tests/framework/cucumber-cpp/lib/libcucumber-cpp.a
    ${CMAKE_SOURCE_DIR}/tests/framework/cgreen/lib/libcgreen.so)

target_include_directories(
    unit-test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/tests/framework/cucumber-cpp/include
    ${CMAKE_SOURCE_DIR}/tests/framework/cgreen/include)


add_custom_target(
    run-unit-tests
    DEPENDS
    unit-test
    COMMAND
    unit-test --port=3902 >/dev/null & cucumber _2.0.0_ ${CMAKE_CURRENT_SOURCE_DIR}/features )
