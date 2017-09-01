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


/* Mocks */
baton_result_t check_relay_argument(
    int * const relay_num,
    yuck_t * const argp )
{
    return (baton_result_t) mock(relay_num, argp);
}

baton_result_t baton_get_relay_status(
    int const fd,
    unsigned int const relay,
    baton_relay_status_t * status )
{
    return (baton_result_t) mock(fd, relay, status);
}


/* Test State */
struct state
{
    int result;
};


GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    int fd = 0;
    yuck_t argp[1];

    if ( function_name == "check_relay_argument()" )
    {
        expect(
            check_relay_argument,
            will_return(BATON_ERROR) );
    }
    else if ( function_name == "baton_get_relay_status()" )
    {
        expect(
            check_relay_argument,
            will_return(BATON_SUCCESS) );

        expect(
            baton_get_relay_status,
            will_return(BATON_ERROR) );
    }

    state->result = parse_read_cmd( fd, argp );
}


THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}
