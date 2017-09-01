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
baton_result_t baton_get_relay_status_by_bitfield(
    int const fd,
    unsigned long * bitfield )
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

    if ( function_name == "baton_get_relay_status_by_bitfield()" )
    {
        expect(
            baton_get_relay_status_by_bitfield,
            will_return(BATON_ERROR) );
    }

    state->result = parse_read_bitfield_cmd( fd );
}


THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}
