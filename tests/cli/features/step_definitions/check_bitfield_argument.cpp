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
    REGEX_PARAM( std::string, bitfield_arg_string );

    ScenarioScope<state> state;

    unsigned long returned_bitfield;
    char bitfield_arg[100] = {0};

    bitfield_arg_string.copy(bitfield_arg, sizeof(bitfield_arg));
    int arg_len = bitfield_arg_string.length();

    /* null terminating byte */
    bitfield_arg[ arg_len ] = '\0';

    state->result = check_bitfield_argument( &returned_bitfield, bitfield_arg );
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
