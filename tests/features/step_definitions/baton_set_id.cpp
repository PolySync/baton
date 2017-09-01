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

baton_result_t baton_get_id(
    int const fd,
    char * const id,
    int const id_length )
{
    return (baton_result_t) mock(fd, id, id_length);
}


/* Test State */
struct state
{
    int fd;
    char id[8] = {'1', '!', ';', '[', '-', '+', '=', '%'};
    baton_result_t result;
};


GIVEN( "^the function is called with invalid \"(.*)\" argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    if ( argument == "id" )
    {
        state->result = baton_set_id( state->fd, " ", sizeof(" ") );
    }
}

GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    if ( function_name == "write_command()" )
    {
        expect(write_command, will_return(BATON_ERROR) );
    }
    else if ( function_name == "baton_get_id()" )
    {
        expect( write_command, will_return(BATON_SUCCESS) );

        expect( baton_get_id, will_return(BATON_ERROR) );
    }

    state->result = baton_set_id( state->fd, state->id, sizeof(state->id) );
}

GIVEN( "^the function completes without error$" )
{
    ScenarioScope<state> state;

    expect( write_command, will_return(BATON_SUCCESS) );

    expect(
        baton_get_id,
        will_return(BATON_SUCCESS),
        will_set_contents_of_parameter( id, &state->id, sizeof(&state->id)) );

    state->result = baton_set_id( state->fd, state->id, sizeof(state->id) );
}

THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that( state->result, is_equal_to(BATON_ERROR) );
}

THEN( "^the function should return success$" )
{
    ScenarioScope<state> state;

    assert_that( state->result, is_equal_to(BATON_SUCCESS) );
}
