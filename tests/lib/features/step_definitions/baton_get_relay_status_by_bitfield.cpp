#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cucumber-cpp/autodetect.hpp>


extern "C"
{
    #include <fcntl.h>
    #include <limits.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <termios.h>
    #include "baton.h"
    #include "internal/baton.h"
}


using cucumber::ScenarioScope;
using namespace cgreen;


/* Mocks */
unsigned long int strtoul(
    const char *nptr,
    char **endptr,
    int base)
{
    return (unsigned long int) mock(nptr, endptr, base);
}

baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length )
{
    return (baton_result_t) mock(fd, command, length);
}

baton_result_t read_response(
    int const fd,
    char * const response,
    int const length )
{
    return (baton_result_t) mock(fd, response, length);
}


/* Test State */
struct state
{
    int fd;
    unsigned long bitfield;
    baton_result_t result;
    char bitfield_from_read_response[5] = "BEEF";
    unsigned long bitfield_from_strtoul = 48879;
};


GIVEN( "^the function is called with null pointer \"(.*)\" argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    if ( argument == "bitfield" )
    {
        state->result =  baton_get_relay_status_by_bitfield(
            state->fd,
            NULL );
    }
}

GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    if ( function_name == "write_command()" )
    {
        expect(
            write_command,
            will_return(BATON_ERROR) );
    }
    else if ( function_name == "read_response()" )
    {
        expect(
            write_command,
            will_return(BATON_SUCCESS) );

        expect(
            read_response,
            will_return(BATON_ERROR) );
    }
    else if ( function_name == "strtoul()" )
    {
        expect(
            write_command,
            will_return(BATON_SUCCESS) );

        expect(
            read_response,
            will_return(BATON_SUCCESS) );

        expect(
            strtoul,
            will_return(ULONG_MAX) );
    }

    state->result = baton_get_relay_status_by_bitfield(
        state->fd,
        &state->bitfield );
}

GIVEN( "^the function completes without error$" )
{
    ScenarioScope<state> state;

    expect(
        write_command,
         will_return(BATON_SUCCESS) );

    expect(
        read_response,
        will_return(BATON_SUCCESS),
        will_set_contents_of_parameter(response, state->bitfield_from_read_response, sizeof(state->bitfield_from_read_response)) );

    expect(
        strtoul,
        will_return(state->bitfield_from_strtoul) );

        state->result = baton_get_relay_status_by_bitfield(
            state->fd,
            &state->bitfield );
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

THEN( "^the function should return a status bitfield$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->bitfield,
        is_equal_to(state->bitfield_from_strtoul) );
}
