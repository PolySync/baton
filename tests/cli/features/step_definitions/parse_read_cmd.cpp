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
    char const * const relay_arg )
{
    return (baton_result_t) mock(relay_num, relay_arg);
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
    size_t arg_count = 1;
    char const *args[] = {"0"};

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

    state->result = parse_read_cmd( fd, arg_count, args );
}


GIVEN( "^(.*) number of arguments are given$" )
{
    REGEX_PARAM( size_t, arg_count );

    ScenarioScope<state> state;

    int fd = 0;

    char const *args[] = {"0"};

    if ( arg_count == 1 )
    {
        expect(
            check_relay_argument,
            will_return(BATON_SUCCESS) );

        expect(
            baton_get_relay_status,
            will_return(BATON_SUCCESS) );
    }

    state->result = parse_read_cmd( fd, arg_count, args );
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
