#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cucumber-cpp/autodetect.hpp>

extern "C"
{
    #include <unistd.h>
    #include "baton.h"
    #include "internal/baton.h"
}


using cucumber::ScenarioScope;
using namespace cgreen;


/* Test State */
struct state
{
    int fd = -1;
    char response[100];
    baton_result_t result;
};


GIVEN( "^the function is called with null pointer \"(.*)\" argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    if ( argument == "response" )
    {
        state->result = read_response(
            state->fd,
            NULL,
            0 );
    }
}

GIVEN( "^a called function returns an error$" )
{
    ScenarioScope<state> state;

    state->result = read_response(
        state->fd,
        state->response,
        sizeof(state->response));
}

THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}
