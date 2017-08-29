/**
 * @file baton.c
 * @brief Baton library.
 */


#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "baton.h"
#include "internal/baton.h"


/**
 * @brief Macro for printing errors to stderr.
 *
 */
#define PRINT_ERROR( message, submessage ) (\
        fprintf(\
            stderr,\
            "ERROR\n"\
            "  FILE: %s\n"\
            "  FUNC: %s\n"\
            "  LINE: %d\n"\
            "  DESC: %s %s\n\n",\
            __FILE__,\
            __FUNCTION__,\
            __LINE__,\
            (message),\
            (submessage) ))


/**
 * @brief Length of transmit and receive buffers.
 *
 */
#define BUFFER_LENGTH ( 25 )


baton_result_t baton_init(
    char const * const port,
    int * const fd )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;
    struct termios tty_config = {0};
    int fd_local;


    if ( (port == NULL)
        || (fd == NULL) )
    {
        PRINT_ERROR( "null pointer argument", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        fd_local = open( port, O_RDWR | O_NOCTTY | O_SYNC );

        if ( fd_local < 0 )
        {
            PRINT_ERROR( "open() error:", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* get current settings */
        ret = tcgetattr( fd_local, &tty_config );

        if ( ret != 0 )
        {
            PRINT_ERROR( "tcgetattr() error:", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* set output baud rate - USB controller on relay doesn't care what speed */
        ret = cfsetospeed( &tty_config, B19200 );

        if ( ret < 0 )
        {
            PRINT_ERROR( "cfsetospeed() error:", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* set output baud rate - USB controller on relay doesn't care what speed */
        ret = cfsetispeed( &tty_config, B19200 );

        if ( ret < 0 )
        {
            PRINT_ERROR( "cfsetispeed() error:", strerror(errno) );

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
        ret = tcsetattr( fd_local, TCSANOW, &tty_config );

        if ( ret != 0 )
        {
            PRINT_ERROR( "tcsetattr() error:", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        *fd = fd_local;
    }


    return result;
}


baton_result_t baton_get_firmware_version(
    int const fd,
    char * const version,
    int const version_length )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;
    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;
    char version_local[BUFFER_LENGTH] = {0};


    if ( version == NULL )
    {
        PRINT_ERROR( "null pointer argument", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'ver' */
        command_length = snprintf( command, sizeof(command), "ver\r" );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = read_response( fd, version_local, sizeof(version_local) );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "read_response() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        memcpy( version, version_local, version_length );
    }


    return result;
}


baton_result_t baton_set_id(
    int const fd,
    unsigned long const id )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;
    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;


    if ( id > 99999999 )
    {
        PRINT_ERROR( "id must be between 0 and 99999999", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'id set XXXXXXXX' where XXXXXXXX is a number
           between 0 and 99999999, padded with zeroes */
        command_length = snprintf( command, sizeof(command), "id set %08lu\r", id );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

            result = BATON_ERROR;
        }
    }


    return result;
}


baton_result_t baton_get_id(
    int const fd,
    char * const id,
    int const id_length )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;
    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;
    char id_local[BUFFER_LENGTH] = {0};


    if ( id == NULL )
    {
        PRINT_ERROR( "null pointer argument", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'id get' */
        command_length = snprintf( command, sizeof(command), "id get\r" );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = read_response( fd, id_local, sizeof(id_local) );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "read_response() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        memcpy( id, id_local, id_length );
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
    char rx_buf[BUFFER_LENGTH] = {0};
    baton_relay_status_t status_local = -1;


    if ( status == NULL )
    {
        PRINT_ERROR( "null pointer argument", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'relay read X' where X is represented as a single
        uppercase hex digit */
        command_length = snprintf( command, sizeof(command), "relay read %X\r", relay );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = read_response( fd, rx_buf, sizeof(rx_buf) );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "read_response() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = strncmp( rx_buf, "on", sizeof(rx_buf) );

        if ( ret == 0 )
        {
            status_local = BATON_RELAY_ON;
        }
        else
        {
            ret = strncmp( rx_buf, "off", sizeof(rx_buf) );

            if ( ret == 0 )
            {
                status_local = BATON_RELAY_OFF;
            }
            else
            {
                PRINT_ERROR( "invalid response from relay", "" );

                result = BATON_ERROR;
            }
        }
    }


    if ( result == BATON_SUCCESS )
    {
        *status = status_local;
    }


    return result;
}


baton_result_t baton_get_relay_status_by_bitfield(
    int const fd,
    unsigned long * const bitfield )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;
    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;
    char rx_buf[BUFFER_LENGTH] = {0};
    unsigned long bitfield_local;


    if ( bitfield == NULL )
    {
        PRINT_ERROR( "null pointer argument", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'relay readall' */
        command_length = snprintf( command, sizeof(command), "relay readall\r" );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = read_response( fd, rx_buf, sizeof(rx_buf) );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "read_response() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        bitfield_local = strtoul( rx_buf, NULL, 16 );

        if ( bitfield_local == ULONG_MAX )
        {
            PRINT_ERROR( "strtoul() error: ", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        *bitfield = bitfield_local;
    }


    return result;
}


baton_result_t baton_enable_relay(
    int const fd,
    unsigned int const relay )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;
    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;
    baton_relay_status_t relay_status;


    result = baton_get_relay_status( fd, relay, &relay_status );

    if ( result != BATON_SUCCESS )
    {
        PRINT_ERROR( "baton_get_relay_status() error", "" );

        result == BATON_ERROR;
    }
    else
    {
        /* don't enable if already enabled */
        if ( relay_status == BATON_RELAY_ON )
        {
            result = BATON_IGNORED;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'relay on X' where X is represented as a single
           uppercase hex digit */
        command_length = snprintf( command, sizeof(command), "relay on %X\r", relay );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

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
    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;
    baton_relay_status_t relay_status;


    result = baton_get_relay_status( fd, relay, &relay_status );

    if (result != BATON_SUCCESS )
    {
        PRINT_ERROR( "baton_get_relay_status() error", "" );

        result = BATON_ERROR;
    }
    else
    {
        /* don't disable if already disabled */
        if ( relay_status == BATON_RELAY_OFF )
        {
            result = BATON_IGNORED;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'relay off X' where X is represented as a single
           uppercase hex digit */
        command_length = snprintf( command, sizeof(command), "relay off %X\r", relay );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

            result = BATON_ERROR;
        }
    }


    return result;
}


baton_result_t baton_toggle_relays_by_bitfield(
    int const fd,
    unsigned int const bitfield )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;
    char command[BUFFER_LENGTH] = {0};
    int command_length = 0;


    if ( bitfield > 0xFFFF )
    {
        PRINT_ERROR( "bitfield must be between 0x0000 and 0xFFFF", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* command is of the form 'relay writeall XXXX' where XXXX is a bitfield
           between 00000 and ffff, where a 1 in bit position N enables relay
           N and a 0 in bit position N disables relay N */
        command_length = snprintf( command, sizeof(command), "relay writeall %04x\r", bitfield );

        if ( command_length < 0 )
        {
            PRINT_ERROR( "snprintf() error", "" );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        ret = write_command( fd, command, command_length );

        if ( ret != BATON_SUCCESS )
        {
            PRINT_ERROR( "write_command() error", "" );

            result = BATON_ERROR;
        }
    }


    return result;
}




/* Internal */
baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    if ( command == NULL )
    {
        PRINT_ERROR( "null pointer argument", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
        /* send a single carriage return to tell the relay to get into
        a state where it is ready to accept further commands */
        ret = write( fd, "\r", 1 );

        if ( ret < 0 )
        {
            PRINT_ERROR( "write() error:", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        char buf[BUFFER_LENGTH];

        /* read back the empty response from the relay and throw it away */
        ret = read( fd, buf, BUFFER_LENGTH );

        if ( ret < 0 )
        {
            PRINT_ERROR( "read() error:", strerror(errno) );

            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        /* write the actual command */
        ret = write( fd, command, length );

        if ( ret < 0 )
        {
            PRINT_ERROR( "write() error:", strerror(errno) );

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
                PRINT_ERROR( "read() error:", strerror(errno) );

                result = BATON_ERROR;
            }
        }
    }


    return result;
}


baton_result_t read_response(
    int const fd,
    char * const response,
    int const length )
{
    baton_result_t result = BATON_SUCCESS;
    int ret = -1;


    if ( response == NULL )
    {
        PRINT_ERROR( "null pointer argument", "" );

        result = BATON_ERROR;
    }


    if ( result == BATON_SUCCESS )
    {
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
                PRINT_ERROR( "read() error:", strerror(errno) );

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
    }


    return result;
}
