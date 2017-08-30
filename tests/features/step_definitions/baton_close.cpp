#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cucumber-cpp/autodetect.hpp>


extern "C"
{
    #include <fcntl.h>
    #include <termios.h>
    #include "baton.h"
}


using cucumber::ScenarioScope;
using namespace cgreen;


/* Mocks */
int close(
    int fd)
{
    return (int) mock(fd);
}

/* Test State */
struct state
{
    int fd = 0;
    baton_result_t result;
};


GIVEN( "^the function is called with null pointer \"(.*)\" argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    if ( argument == "fd" )
    {
        state->result = baton_close( -1 );
    }
}

GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    if ( function_name == "close()" )
    {
        expect(
            close,
            will_return(-1) );
    }

    state->result = baton_close( state->fd );
}

GIVEN( "^the function completes without error$" )
{
    ScenarioScope<state> state;

    expect(
        close,
        will_return(0) );

    state->result = baton_close(
        state->fd );
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
