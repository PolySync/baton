#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
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
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    struct termios tty_config;
    memset( &tty_config, 0, sizeof( tty_config) );


    *fd = open( port, O_RDWR | O_NOCTTY | O_SYNC );

    if ( *fd < 0 )
    {
        printf( "open() error: %s\n", strerror(errno) );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* get current settings */
        ret = tcgetattr( *fd, &tty_config );

        if ( ret != 0 )
        {
            printf( "tcgetattr() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* set output baud rate */
        ret = cfsetospeed( &tty_config, speed );

        if ( ret != 0 )
        {
            printf( "cfsetospeed() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* set input  baud rate */
        ret = cfsetispeed( &tty_config, speed );

        if ( ret != 0 )
        {
            printf( "cfsetispeed() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
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


baton_result_t baton_get_firmware_version(
    int const fd,
    char * const rx_buf,
    int const rx_buf_length )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    if ( rx_buf == NULL )
    {
        printf( "baton_get_firmware_version(): rx_buf is a null pointer\n");

        result = BATON_ERROR;
    }


    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;

    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'ver' */
        command_length = snprintf( command, sizeof(command), "ver\r" );

        if ( command_length < 0 )
        {
            printf( "snprintf() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            printf( "write_command() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = read_response( fd, rx_buf, rx_buf_length );

        if ( ret != BATON_SUCCESS )
        {
            printf( "read_response() error\n" );

            result = BATON_ERROR;
        }
    }


    return result;
}


baton_result_t baton_set_id(
    int const fd,
    unsigned long const id )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    if ( id > 99999999 )
    {
        printf( "baton_set_id(): ID must be between 0 and 99999999\n" );

        result = BATON_ERROR;
    }


    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;

    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'id set XXXXXXXX' where XXXXXXXX is a number
           between 0 and 99999999, padded with zeroes */
        command_length = snprintf( command, sizeof(command), "id set %08lu\r", id );

        if ( command_length < 0 )
        {
            printf( "snprintf() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if( ret != BATON_SUCCESS )
        {
            printf( "write_command() error\n" );

            result = BATON_ERROR;
        }
    }


    return result;
}


baton_result_t baton_get_id(
    int const fd,
    char * const rx_buf,
    int const rx_buf_length )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    if ( rx_buf == NULL )
    {
        printf( "baton_get_id(): rx_buf is a null pointer\n");

        result = BATON_ERROR;
    }


    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;

    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'id get' */
        command_length = snprintf( command, sizeof(command), "id get\r" );

        if ( command_length < 0 )
        {
            printf( "snprintf() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            printf( "write_command() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = read_response( fd, rx_buf, rx_buf_length );

        if ( ret != BATON_SUCCESS )
        {
            printf( "read_response() error\n" );

            result = BATON_ERROR;
        }
    }


    return result;
}


baton_result_t baton_get_relay_status(
    int const fd,
    unsigned int const relay,
    baton_relay_status_t *status )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;

    /* command is of the form 'relay read X' where X is represented as a single
       uppercase hex digit */
    command_length = snprintf( command, sizeof(command), "relay read %X\r", relay );

    if ( command_length < 0 )
    {
        printf( "snprintf() error\n" );

        result = BATON_ERROR;
    }

    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if( ret != BATON_SUCCESS )
        {
            printf( "write_command() error\n" );

            result = BATON_ERROR;
        }
    }

    char rx_buf[BUFFER_LENGTH] = {0};

    if ( result == BATON_SUCCESS )
    {
        ret = read_response( fd, rx_buf, sizeof(rx_buf) );

        if ( ret != BATON_SUCCESS )
        {
            printf( "read_response() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = strncmp( rx_buf, "on", sizeof(rx_buf) );

        if ( ret == 0 )
        {
            *status = BATON_RELAY_ON;
        }
        else
        {
            ret = strncmp( rx_buf, "off", sizeof(rx_buf) );

            if ( ret == 0 )
            {
                *status = BATON_RELAY_OFF;
            }
            else
            {
                printf( "baton_get_relay_status(): invalid response from relay\n" );

                result = BATON_ERROR;
            }
        }
    }


    return result;
}


baton_result_t baton_enable_relay(
    int const fd,
    unsigned int const relay )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    baton_relay_status_t relay_status;

    result = baton_get_relay_status( fd, relay, &relay_status );

    if ( result != BATON_SUCCESS )
    {
        printf( "baton_enable_relay(): could not query relay for current status\n" );
    }
    else
    {
        /* don't enable if already enabled */
        if ( relay_status == BATON_RELAY_ON )
        {
            result = BATON_IGNORED;
        }
    }


    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;

    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'relay on X' where X is represented as a single
           uppercase hex digit */
        command_length = snprintf( command, sizeof(command), "relay on %X\r", relay );

        if ( command_length < 0 )
        {
            printf( "snprintf() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            printf( "write_command() error\n" );

            result = BATON_ERROR;
        }
    }


    return result;
}


baton_result_t baton_disable_relay(
    int const fd,
    unsigned int const relay )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    baton_relay_status_t relay_status;

    result = baton_get_relay_status( fd, relay, &relay_status );

    if (result != BATON_SUCCESS )
    {
        printf( "baton_disable_relay(): could not query relay for current status\n" );
    }
    else
    {
        /* don't disable if already disabled */
        if ( relay_status == BATON_RELAY_OFF )
        {
            result = BATON_IGNORED;
        }
    }


    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;

    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'relay off X' where X is represented as a single
           uppercase hex digit */
        command_length = snprintf( command, sizeof(command), "relay off %X\r", relay );

        if ( command_length < 0 )
        {
            printf( "snprintf() error\n" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            printf( "write_command() error\n" );

            result = BATON_ERROR;
        }
    }


    return result;
}


static baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;

    /* send a single carriage return to tell the relay to get into
       a state where it is ready to accept further commands */
    ret = write( fd, "\r", 1 );

    if ( ret < 0 )
    {
        printf( "write() error: %s\n", strerror(errno) );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
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


    if ( result == BATON_SUCCESS )
    {
        /* write the actual command */
        ret = write( fd, command, length );

        if ( ret < 0 )
        {
            printf( "write() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* the relay echoes every character sent to it ending in a carriage
           return, all of which must be discarded */
        char read_byte = 0;

        while( (read_byte != '\r')
                && (result == BATON_SUCCESS) )
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
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    /* the response must be read one byte at a time until a line feed is
       read, indicating the end of the response */
    char read_byte = 0;
    int response_index = 0;

    while( (read_byte != '>')
            && (result == BATON_SUCCESS) )
    {
        ret = read( fd, &read_byte, 1 );

        if ( ret < 0 )
        {
            printf( "read() error: %s\n", strerror(errno) );

            result = BATON_ERROR;
        }
        else
        {
            /* if byte is not alphanumeric, discard */
            ret = isalnum( read_byte );

            if ( ret != 0 )
            {
                /* do not write a response longer than the buffer can hold */
                if ( response_index < length )
                {
                    response[response_index] = read_byte;
                    ++response_index;
                }
            }
        }
    }


    return result;
}
