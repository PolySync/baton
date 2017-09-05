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
baton_result_t check_bitfield_argument(
    unsigned long * const bitfield,
    yuck_t const * const argp )
{
    return (baton_result_t) mock(bitfield, argp);
}

baton_result_t baton_toggle_relays_by_bitfield(
    int const fd,
    unsigned long const bitfield )
{
    return (baton_result_t) mock(fd, bitfield);
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

    if ( function_name == "check_bitfield_argument()" )
    {
        expect(
            check_bitfield_argument,
            will_return(BATON_ERROR) );
    }
    else if ( function_name == "baton_toggle_relays_by_bitfield()" )
    {
        expect(
            check_bitfield_argument,
            will_return(BATON_SUCCESS) );

        expect(
            baton_toggle_relays_by_bitfield,
            will_return(BATON_ERROR) );
    }

    state->result = parse_toggle_bitfield_cmd( fd, argp );
}


THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}
