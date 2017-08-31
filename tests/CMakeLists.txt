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


# generate random port for test to avoid conflicts
string(
    RANDOM
    LENGTH 3
    ALPHABET "0123456789"
    PORT_SUFFIX)

set(
    CUCUMBER_PORT
    "25${PORT_SUFFIX}")

file(
    GLOB
    STEP_DEFINITION_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/features/step_definitions/*.cpp)

add_custom_target(
    test-steps
    DEPENDS
    libbaton-shared)

add_custom_target(
    run-unit-tests
    DEPENDS
    test-steps)

# build individual feature executables
foreach(STEP_DEFINITION_SRC_FILE ${STEP_DEFINITION_SRCS})
    # get file name, the foo in foo.c
    get_filename_component(
        STEP_DEFINITION_FILE_NAME
        ${STEP_DEFINITION_SRC_FILE}
        NAME_WE)

    string(
        REPLACE "_" "-"
        STEP_DEFINITION_SOURCE_FILE_TARGET
        ${STEP_DEFINITION_FILE_NAME})

    set(
        STEP_TARGET
        ${STEP_DEFINITION_SOURCE_FILE_TARGET}-steps)

    add_executable(
        ${STEP_TARGET}
        EXCLUDE_FROM_ALL
        ${STEP_DEFINITION_SRC_FILE})

    add_dependencies(
        test-steps
        ${STEP_TARGET})

    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/features/step_definitions/cucumber.wire "host: localhost\nport: ${CUCUMBER_PORT}")

    add_custom_target(
        run-${STEP_DEFINITION_SOURCE_FILE_TARGET}-unit-test
        DEPENDS
        ${STEP_TARGET}
        COMMAND
        ${STEP_TARGET} --port=${CUCUMBER_PORT} >/dev/null 2>&1 & cucumber _2.0.0_ ${CMAKE_CURRENT_SOURCE_DIR}/features/${STEP_DEFINITION_FILE_NAME}.feature)

    add_dependencies(
        run-unit-tests
        run-${STEP_DEFINITION_SOURCE_FILE_TARGET}-unit-test)

    target_compile_options(
        ${STEP_TARGET}
        PRIVATE
        -std=c++11
        -Wall)

    target_include_directories(
        ${STEP_TARGET}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/tests/framework/cucumber-cpp/include
        ${CMAKE_SOURCE_DIR}/tests/framework/cgreen/include)

    target_link_libraries(
        ${STEP_TARGET}
        PRIVATE
        libbaton-shared
        ${CMAKE_SOURCE_DIR}/tests/framework/cucumber-cpp/lib/libcucumber-cpp.a
        ${CMAKE_SOURCE_DIR}/tests/framework/cgreen/lib/libcgreen.so)

endforeach(STEP_DEFINITION_SRC_FILE)
