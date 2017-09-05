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
    char const * const id_arg )
{
    return (baton_result_t) mock(id, id_arg);
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
    size_t arg_count = 1;
    char const *args[] = {"00000000"};

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

    state->result = parse_set_id_cmd( fd, arg_count, args );
}


GIVEN( "^(.*) number of arguments are given$" )
{
    REGEX_PARAM( size_t, arg_count );

    ScenarioScope<state> state;

    int fd = 0;

    char const *args[] = {"00000000"};

    if ( arg_count == 1 )
    {
        expect(
            check_id_argument,
            will_return(BATON_SUCCESS) );

        expect(
            baton_set_id,
            will_return(BATON_SUCCESS) );
    }

    state->result = parse_set_id_cmd( fd, arg_count, args );
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
