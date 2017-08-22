#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "baton.h"


baton_result_t baton_init( char const * const port, int const speed, int * const fd )
{
    int result = BATON_SUCCESS;
    int ret = -1;


    struct termios tty_config;
    memset( &tty_config, 0, sizeof( tty_config) );


    *fd = open( port, O_RDWR | O_NOCTTY | O_SYNC );

    if ( *fd < 0 )
    {
        printf( "open() error: %s\n", strerror(errno) );

        result = BATON_ERROR;
    }


    if ( result != BATON_ERROR )
    {
        /* get current settings */
        ret = tcgetattr( *fd, &tty_config );

        if ( ret != 0 )
        {
            printf( "tcgetattr() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result != BATON_ERROR )
    {
        /* set output baud rate */
        ret = cfsetospeed( &tty_config, speed );

        if ( ret != 0 )
        {
            printf( "cfsetospeed() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result != BATON_ERROR )
    {
        /* set input  baud rate */
        ret = cfsetispeed( &tty_config, speed );

        if ( ret != 0 )
        {
            printf( "cfsetispeed() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result != BATON_ERROR )
    {
        /* use raw input and output with no byte read minimum and a timeout of
           half of a second */
        cfmakeraw(&tty_config);
        tty_config.c_cc[VMIN]  = 0;
        tty_config.c_cc[VTIME] = 5;

        /* apply settings immediately */
        ret = tcsetattr( *fd, TCSANOW, &tty_config );

        if ( ret != 0 )
        {
            printf( "tcsetattr() error: %s\n", strerror(errno) );
        }
    }


    return result;
}
