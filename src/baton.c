#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "baton.h"


/**
 * @brief Length of transmit and receive buffers.
 *
 */
#define BUFFER_LENGTH ( 25 )


static baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length );

static baton_result_t read_response(
    int const fd,
    char * const response,
    int const length );


baton_result_t baton_init(
    char const * const port,
    int const speed,
    int * const fd )
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


static baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length )
{
    int result = BATON_SUCCESS;
    int ret = -1;

    /* send a single carriage return to tell the relay to get into
       a state where it is ready to accept further commands */
    ret = write( fd, "\r", 1 );

    if ( ret < 0 )
    {
        printf( "write() error: %s\n", strerror(errno) );

        result = BATON_ERROR;
    }


    if ( result != BATON_ERROR )
    {
        char buf[BUFFER_LENGTH];

        /* read back the empty response from the relay and throw it away */
        ret = read( fd, buf, BUFFER_LENGTH );

        if ( ret < 0 )
        {
            printf( "read() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result != BATON_ERROR )
    {
        /* write the actual command */
        ret = write( fd, command, length );

        if ( ret < 0 )
        {
            printf( "write() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result != BATON_ERROR )
    {
        /* the relay echoes every character sent to it ending in a carriage
           return, all of which must be discarded */
        char read_byte = 0;

        while( (read_byte != '\r')
                && (result != BATON_ERROR) )
        {
            ret = read( fd, &read_byte, 1 );

            if ( ret < 0 )
            {
                printf( "read() error: %s\n", strerror(errno) );

                result = BATON_ERROR;
            }
        }
    }


    return result;
}


static baton_result_t read_response(
    int const fd,
    char * const response,
    int const length )
{
    int result = BATON_SUCCESS;
    int ret = -1;


    /* the response must be read one byte at a time until a line feed is
       read, indicating the end of the response */
    char read_byte = 0;
    int response_index = 0;

    while( (read_byte != '\n')
            && (result != BATON_ERROR) )
    {
        ret = read( fd, &read_byte, 1 );

        if ( ret < 0 )
        {
            printf( "read() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
        else if (read_byte != '\n')
        {
            /* do not write a response longer than the buffer can hold */
            if ( response_index < length )
            {
                response[response_index] = read_byte;
                ++response_index;
            }
        }
    }


    return result;
}
