#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cucumber-cpp/autodetect.hpp>

#include <fcntl.h>
#include <termios.h>

extern "C"
{
    #include "baton.h"
}


using cucumber::ScenarioScope;
using namespace cgreen;


/* Mocks */
void cfmakeraw(
    struct termios *termios_p)
{
    return (void) mock(termios_p);
}

int cfsetispeed(
    struct termios *termios_p,
    speed_t speed)
{
    return (int) mock(termios_p, speed);
}

int cfsetospeed(
    struct termios *termios_p,
    speed_t speed)
{
    return (int) mock(termios_p, speed);
}

int open(
    const char *pathname,
    int flags,
    ...)
{
    return (int) mock(pathname, flags);
}

int tcgetattr(
    int fd,
    struct termios *termios_p)
{
    return (int) mock(fd, termios_p);
}

int tcsetattr(
    int fd,
    int optional_actions,
    const struct termios *termios_p)
{
    return (int) mock(fd, optional_actions, termios_p);
}


/* Test State */
struct state
{
    int fd;
    int speed = B9600;
    baton_result_t result;
    int fd_from_open = 5;
};


GIVEN( "^the function is called with null pointer \"(.*)\" argument$" )
{
    REGEX_PARAM( std::string, argument );

    ScenarioScope<state> state;

    if ( argument == "port" )
    {
        state->result = baton_init(
            NULL,
            state->speed,
            &state->fd );
    }
    else if ( argument == "fd" )
    {
        state->result = baton_init(
            "/dev/null",
            state->speed,
            NULL );
    }
}

GIVEN( "^(.*) returns an error$" )
{
    REGEX_PARAM( std::string, function_name );

    ScenarioScope<state> state;

    if ( function_name == "open()" )
    {
        expect(
            open,
            will_return(-1) );
    }
    else if ( function_name == "tcgetattr()" )
    {
        expect(
            open,
            will_return(0) );

        expect(
            tcgetattr,
            will_return(-1) );
    }
    else if ( function_name == "cfsetospeed()" )
    {
        expect(
            open,
            will_return(0) );

        expect(
            tcgetattr,
            will_return(0) );

        expect(
            cfsetospeed,
            will_return(-1) );
    }
    else if ( function_name == "cfsetispeed()" )
    {
        expect(
            open,
            will_return(0) );

        expect(
            tcgetattr,
            will_return(0) );

        expect(
            cfsetospeed,
            will_return(state->speed) );

        expect(
            cfsetispeed,
            will_return(-1) );
    }
    else if ( function_name == "tcsetattr()" )
    {
        expect(
            open,
            will_return(0) );

        expect(
            tcgetattr,
            will_return(0) );

        expect(
            cfsetospeed,
            will_return(state->speed) );

        expect(
            cfsetispeed,
            will_return(state->speed) );

        expect(
            cfmakeraw );

        expect(
            tcsetattr,
            will_return(-1) );
    }

    state->result = baton_init(
        "/dev/null",
        state->speed,
        &state->fd );
}

GIVEN( "^the function completes without error$" )
{
    ScenarioScope<state> state;

    expect(
        open,
        will_return(state->fd_from_open) );

    expect(
        tcgetattr,
        will_return(0) );

    expect(
        cfsetospeed,
        will_return(state->speed) );

    expect(
        cfsetispeed,
        will_return(state->speed) );

    expect(
        cfmakeraw );

    expect(
        tcsetattr,
        will_return(0) );

    state->result = baton_init(
        "/dev/null",
        state->speed,
        &state->fd );
}

THEN( "^the function should return an error$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_ERROR) );
}

THEN( "^the function should return success$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->result,
        is_equal_to(BATON_SUCCESS) );
}

THEN( "^the function should return a file descriptor$" )
{
    ScenarioScope<state> state;

    assert_that(
        state->fd,
        is_equal_to(state->fd_from_open) );
}
