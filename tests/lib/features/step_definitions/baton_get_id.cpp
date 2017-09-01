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
    char id[100];
    baton_result_t result;
    char id_from_read_response[9] = "01234567";
};


GIVEN( "^the function is called with null pointer \"(.*)\" argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    if ( argument == "rx_buf" )
    {
        state->result = baton_get_id(
            state->fd,
            NULL,
            0 );
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

    state->result = baton_get_id(
        state->fd,
        state->id,
        sizeof(state->id) );
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
        will_set_contents_of_parameter(response, state->id_from_read_response, sizeof(state->id_from_read_response)) );

    state->result = baton_get_id(
        state->fd,
        state->id,
        sizeof(state->id) );
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

THEN( "^the function should return an id$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->id,
        is_equal_to_string(state->id_from_read_response) );
}
