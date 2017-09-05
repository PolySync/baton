#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cucumber-cpp/autodetect.hpp>


extern "C"
{
    #include <fcntl.h>
    #include <termios.h>
    #include "baton.h"
    #include "internal/main.h"

    int test_main( int argc, char ** argv );
}


using cucumber::ScenarioScope;
using namespace cgreen;


/* Mocks */
baton_result_t baton_open(
    char const * const port,
    int * const fd )
{
    return (baton_result_t) mock(port, fd);
}

baton_result_t baton_close(
    int const fd )
{
    return (baton_result_t) mock(fd);
}

baton_result_t parse_enable_cmd(
    int const fd,
    yuck_t const * const argp )
{
    return (baton_result_t) mock(fd, argp);
}

baton_result_t parse_disable_cmd(
    int const fd,
    yuck_t const * const argp )
{
    return (baton_result_t) mock(fd, argp);
}

baton_result_t parse_read_cmd(
    int const fd,
    yuck_t const * const argp )
{
    return (baton_result_t) mock(fd, argp);
}

baton_result_t parse_set_id_cmd(
    int const fd,
    yuck_t const * const argp )
{
    return (baton_result_t) mock(fd, argp);
}

baton_result_t parse_get_id_cmd(
    int const fd )
{
    return (baton_result_t) mock(fd);
}

baton_result_t parse_get_firmware_version_cmd(
    int const fd )
{
    return (baton_result_t) mock(fd);
}

baton_result_t parse_toggle_bitfield_cmd(
    int const fd,
    yuck_t const * const argp )
{
    return (baton_result_t) mock(fd, argp);
}

baton_result_t parse_read_bitfield_cmd(
    int const fd )
{
    return (baton_result_t) mock(fd);
}


/* Test State */
struct state
{
    int result;
};


GIVEN( "^the CLI is executed with no arguments$" )
{
    ScenarioScope<state> state;

    int argc = 1;
    char * argv[1];

    char arg0[100];

    strncpy( arg0, "baton", sizeof(arg0) );

    argv[0] = arg0;

    state->result = test_main( argc, argv );
}


GIVEN( "^the CLI is executed with only (.*) argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    int argc = 3;
    char * argv[3];

    if ( argument == "DEVICE" )
    {
        argc = 3;

        char arg0[100];
        char arg1[100];
        char arg2[100];

        strncpy( arg0, "baton", sizeof(arg0) );
        strncpy( arg1, "-d", sizeof(arg1) );
        strncpy( arg2, "/dev/ttyACM0", sizeof(arg2) );

        argv[0] = arg0;
        argv[1] = arg1;
        argv[2] = arg2;
    }
    else if ( argument == "COMMAND" )
    {
        argc = 2;

        char arg0[100];
        char arg1[100];

        strncpy( arg0, "baton", sizeof(arg0) );
        strncpy( arg1, "enable", sizeof(arg1) );

        argv[0] = arg0;
        argv[1] = arg1;
    }

    state->result = test_main( argc, argv );
}

GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    int argc = 4;
    char * argv[4];

    char arg0[100];
    char arg1[100];
    char arg2[100];
    char arg3[100];

    strncpy( arg0, "baton", sizeof(arg0) );
    strncpy( arg1, "-d", sizeof(arg1) );
    strncpy( arg2, "/dev/ttyACM0", sizeof(arg2) );

    argv[0] = arg0;
    argv[1] = arg1;
    argv[2] = arg2;

    if ( function_name == "baton_open()" )
    {
        strncpy( arg3, "enable", sizeof(arg3) );
        argv[3] = arg3;

        expect(
            baton_open,
            will_return(BATON_ERROR) );
    }
    else if ( function_name == "parse_enable_cmd()" )
    {
        strncpy( arg3, "enable", sizeof(arg3) );
        argv[3] = arg3;


        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_enable_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "parse_disable_cmd()" )
    {
        strncpy( arg3, "disable", sizeof(arg3) );
        argv[3] = arg3;


        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_disable_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "parse_read_cmd()" )
    {
        strncpy( arg3, "read", sizeof(arg3) );
        argv[3] = arg3;


        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_read_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "parse_set_id_cmd()" )
    {
        strncpy( arg3, "set_id", sizeof(arg3) );
        argv[3] = arg3;


        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_set_id_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "parse_get_id_cmd()" )
    {
        strncpy( arg3, "get_id", sizeof(arg3) );
        argv[3] = arg3;


        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_get_id_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "parse_get_firmware_version_cmd()" )
    {
        strncpy( arg3, "get_firmware_version", sizeof(arg3) );
        argv[3] = arg3;


        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_get_firmware_version_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "parse_toggle_bitfield_cmd()" )
    {
        strncpy( arg3, "toggle_bitfield", sizeof(arg3) );
        argv[3] = arg3;


        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_toggle_bitfield_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "parse_read_bitfield_cmd()" )
    {
        strncpy( arg3, "read_bitfield", sizeof(arg3) );
        argv[3] = arg3;

        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_read_bitfield_cmd,
            will_return(BATON_ERROR) );

        expect(
            baton_close,
            will_return(BATON_SUCCESS) );
    }
    else if ( function_name == "baton_close()" )
    {
        strncpy( arg3, "enable", sizeof(arg3) );
        argv[3] = arg3;

        expect(
            baton_open,
            will_return(BATON_SUCCESS) );

        expect(
            parse_enable_cmd,
            will_return(BATON_SUCCESS) );

        expect(
            baton_close,
            will_return(BATON_ERROR) );
    }

    state->result = test_main( argc, argv );
}


THEN( "^the CLI should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}
