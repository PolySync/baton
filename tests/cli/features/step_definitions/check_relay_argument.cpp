#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cucumber-cpp/autodetect.hpp>


extern "C"
{
    #include <fcntl.h>
    #include <termios.h>
    #include "baton.h"
    #include "internal/main.h"
}


using cucumber::ScenarioScope;
using namespace cgreen;


/* Test State */
struct state
{
    int result;
};


GIVEN( "^argument (.*) is given to function$" )
{
    REGEX_PARAM( std::string, relay_arg_string );

    ScenarioScope<state> state;

    int returned_relay;
    char relay_arg[100] = {0};

    relay_arg_string.copy(relay_arg, sizeof(relay_arg));
    int arg_len = relay_arg_string.length();

    /* null terminating byte */
    relay_arg[ arg_len ] = '\0';

    state->result = check_relay_argument( &returned_relay, relay_arg );
}


THEN( "^the function should return (.*)$" )
{
    REGEX_PARAM( std::string, return_code );

    ScenarioScope<state> state;

    if ( return_code == "BATON_ERROR" )
    {
        assert_that(
            state->result,
            is_equal_to(BATON_ERROR) );
    }
    else if ( return_code == "BATON_SUCCESS" )
    {
        assert_that(
            state->result,
            is_equal_to(BATON_SUCCESS) );
    }
}
