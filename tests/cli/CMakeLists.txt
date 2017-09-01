project(cli-unit-tests)


file(
    GLOB
    STEP_DEFINITION_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/features/step_definitions/*.cpp)

add_custom_target(
    test-cli-steps
    DEPENDS
    libbaton-shared
    cli-shared)

add_custom_target(
    run-cli-unit-tests
    DEPENDS
    test-cli-steps)

# build individual feature executables
foreach(STEP_DEFINITION_SRC_FILE ${STEP_DEFINITION_SRCS})
    # get file name, the foo in foo.c
    get_filename_component(
        STEP_DEFINITION_FILE_NAME
        cli-${STEP_DEFINITION_SRC_FILE}
        NAME_WE)

    string(
        REPLACE "_" "-"
        STEP_DEFINITION_SOURCE_FILE_TARGET
        ${STEP_DEFINITION_FILE_NAME})

    set(
        STEP_TARGET
        test-cli-${STEP_DEFINITION_SOURCE_FILE_TARGET}-steps)

    add_executable(
        ${STEP_TARGET}
        EXCLUDE_FROM_ALL
        ${STEP_DEFINITION_SRC_FILE})

    add_dependencies(
        test-cli-steps
        ${STEP_TARGET})

    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/features/step_definitions/cucumber.wire "host: localhost\nport: ${CUCUMBER_PORT}")

    add_custom_target(
        run-cli-${STEP_DEFINITION_SOURCE_FILE_TARGET}-unit-test
        DEPENDS
        ${STEP_TARGET}
        COMMAND
        ${STEP_TARGET} --port=${CUCUMBER_PORT} >/dev/null 2>&1 & cucumber _2.0.0_ ${CMAKE_CURRENT_SOURCE_DIR}/features/${STEP_DEFINITION_FILE_NAME}.feature)

    add_dependencies(
        run-cli-unit-tests
        run-cli-${STEP_DEFINITION_SOURCE_FILE_TARGET}-unit-test)

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
        cli-shared
        ${CMAKE_SOURCE_DIR}/tests/framework/cucumber-cpp/lib/libcucumber-cpp.a
        ${CMAKE_SOURCE_DIR}/tests/framework/cgreen/lib/libcgreen.so)

endforeach(STEP_DEFINITION_SRC_FILE)
