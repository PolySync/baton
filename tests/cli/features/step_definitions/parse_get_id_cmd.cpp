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
    int result;
};


GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    int fd = 0;

    if ( function_name == "baton_get_id()" )
    {
        expect(
            baton_get_id,
            will_return(BATON_ERROR) );
    }

    state->result = parse_get_id_cmd( fd );
}


THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}
