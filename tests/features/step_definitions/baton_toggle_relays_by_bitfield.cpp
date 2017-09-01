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
baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length )
{
    return (baton_result_t) mock(fd, command, length);
}

baton_result_t baton_get_relay_status_by_bitfield(
    int const fd,
    unsigned long * const bitfield )
{
    return (baton_result_t) mock(fd, bitfield);
}


/* Test State */
struct state
{
    int fd;
    unsigned int relay;
    unsigned long bitfield = 0xBEEF;
    baton_result_t result;
};


GIVEN( "^the function is called with invalid \"(.*)\" argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    if ( argument == "bitfield" )
    {
        state->result = baton_toggle_relays_by_bitfield( state->fd, 0x10000 );
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
    else if ( function_name == "baton_get_relay_status_by_bitfield()" )
    {
        expect(
            write_command,
            will_return(BATON_SUCCESS) );

        expect(
            baton_get_relay_status_by_bitfield,
            will_return(BATON_ERROR) );
    }

    state->result = baton_toggle_relays_by_bitfield(
        state->fd,
        state->bitfield );
}

GIVEN( "^the function completes without error$" )
{
    ScenarioScope<state> state;

    expect(
        write_command,
        will_return(BATON_SUCCESS) );

    expect(
        baton_get_relay_status_by_bitfield,
        will_return(BATON_SUCCESS),
        will_set_contents_of_parameter( bitfield, &state->bitfield, sizeof(&state->bitfield)) );

    state->result = baton_toggle_relays_by_bitfield(
        state->fd,
        state->bitfield );
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
