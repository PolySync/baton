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
    REGEX_PARAM( std::string, id_arg_string );

    ScenarioScope<state> state;

    char returned_id[100];
    char id_arg[100] = {0};

    id_arg_string.copy(id_arg, sizeof(id_arg));
    int arg_len = id_arg_string.length();

    /* null terminating byte */
    id_arg[ arg_len ] = '\0';

    state->result = check_id_argument( returned_id, id_arg );
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
