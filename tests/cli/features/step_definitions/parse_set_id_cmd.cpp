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
baton_result_t check_id_argument(
    char * const id,
    yuck_t const * const argp )
{
    return (baton_result_t) mock(id, argp);
}

baton_result_t baton_set_id(
    int const fd,
    char const * const id,
    int const id_length )
{
    return (baton_result_t) mock(fd, id, id_length );
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

    if ( function_name == "check_id_argument()" )
    {
        expect(
            check_id_argument,
            will_return(BATON_ERROR) );
    }
    else if ( function_name == "baton_set_id()" )
    {
        expect(
            check_id_argument,
            will_return(BATON_SUCCESS) );

        expect(
            baton_set_id,
            will_return(BATON_ERROR) );
    }

    state->result = parse_set_id_cmd( fd, argp );
}


THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}
