#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cucumber-cpp/autodetect.hpp>


extern "C"
{
    #include <fcntl.h>
    #include <stdio.h>
    #include <termios.h>
    #include "baton.h"
    #include "internal/baton.h"
}


using cucumber::ScenarioScope;
using namespace cgreen;


/* Mocks */
baton_result_t baton_get_relay_status(
    int const fd,
    unsigned int const relay,
    baton_relay_status_t * status )
{
    return (baton_result_t) mock(fd, relay, status);
}

baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length )
{
    return (baton_result_t) mock(fd, command, length);
}


/* Test State */
struct state
{
    int fd;
    unsigned int relay;
    baton_result_t result;
};


GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    if ( function_name == "baton_get_relay_status()" )
    {
        expect(
            baton_get_relay_status,
            will_return(BATON_ERROR) );
    }
    else if ( function_name == "write_command()" )
    {
        expect(
            baton_get_relay_status,
            will_return(BATON_SUCCESS) );

        expect(
            write_command,
            will_return(BATON_ERROR) );
    }

    state->result = baton_enable_relay(
        state->fd,
        0 );
}

GIVEN( "^the function completes without error$" )
{
    ScenarioScope<state> state;

    expect(
        baton_get_relay_status,
        will_return(BATON_SUCCESS) );

    expect(
        write_command,
        will_return(BATON_SUCCESS) );

    state->result = baton_enable_relay(
        state->fd,
        0 );
}

THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}

THEN( "^the function should return success$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_SUCCESS) );
}
