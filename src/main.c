/**
 * @file main.c
 * @brief Baton command line interface.
 */


#include <stdlib.h>
#include <termios.h>
#include "baton.h"
#include "internal/args.h"
#include "internal/main.h"


int main(
    int argc,
    char ** argv)
{
    int ret = -1;
    baton_result_t result = BATON_ERROR;
    yuck_t argp[1];
    int fd;


    yuck_parse(argp, argc, argv);


    if ( argp->device_arg == NULL )
    {
        yuck_auto_help( argp );
    }
    else
    {
        result = baton_open( argp->device_arg, &fd );
    }


    if ( result == BATON_SUCCESS )
    {
        if ( argp->cmd == BATON_CMD_ENABLE )
        {
            result = parse_enable_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_DISABLE )
        {
            result = parse_disable_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_READ )
        {
            result = parse_read_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_SET_ID )
        {
            result = parse_set_id_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_GET_ID )
        {
            result = parse_get_id_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_GET_FIRMWARE_VERSION )
        {
            result = parse_get_firmware_version_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_TOGGLE_BITFIELD )
        {
            result = parse_toggle_bitfield_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_READ_BITFIELD )
        {
            result = parse_read_bitfield_cmd( fd, argp );
        }

        baton_close( fd );
    }


    yuck_free(argp);


    return result;
}




/* Internal */
baton_result_t parse_enable_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;


    if (argp->nargs != 1)
    {
        yuck_auto_help( argp );
    }
    else
    {
        int relay = atoi( argp->args[0] );
        result = baton_enable_relay( fd, relay );
    }


    return result;
}


baton_result_t parse_disable_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;


    if (argp->nargs != 1)
    {
        yuck_auto_help( argp );
    }
    else
    {
        int relay = atoi( argp->args[0] );
        result = baton_disable_relay( fd, relay );
    }


    return result;
}


baton_result_t parse_read_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;


    if (argp->nargs != 1)
    {
        yuck_auto_help( argp );
    }
    else
    {
        int relay = atoi( argp->args[0] );
        baton_relay_status_t status;
        result = baton_get_relay_status( fd, relay, &status );

        if ( result == BATON_SUCCESS )
        {
            printf( "Relay %d: ", relay );

            if ( status == BATON_RELAY_ON )
            {
                printf( "ON\n" );
            }
            else
            {
                printf( "OFF\n" );
            }
        }
    }


    return result;
}


baton_result_t parse_set_id_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;


    if (argp->nargs != 1)
    {
        yuck_auto_help( argp );
    }
    else
    {
        char * id = argp->args[0];

        result = baton_set_id( fd, id, sizeof(id) );
    }


    return result;
}


baton_result_t parse_get_id_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;
    char id[9];


    result = baton_get_id( fd, id, sizeof(id) );

    if ( result == BATON_SUCCESS )
    {
        printf( "ID: %s\n", id );
    }


    return result;
}


baton_result_t parse_get_firmware_version_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;
    char version[9];


    result = baton_get_firmware_version(fd, version, sizeof(version) );

    if ( result == BATON_SUCCESS )
    {
        printf( "Firmware: %s\n", version );
    }


    return result;
}


baton_result_t parse_toggle_bitfield_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;


    if ( argp->nargs != 1 )
    {
        yuck_auto_help( argp );
    }
    else
    {
        unsigned int bitfield = strtoul( argp->args[0], NULL, 16 );

        result = baton_toggle_relays_by_bitfield( fd, bitfield );
    }


    return result;
}


baton_result_t parse_read_bitfield_cmd( int fd, yuck_t *argp )
{
    baton_result_t result = BATON_ERROR;
    unsigned int bitfield;


    result = baton_get_relay_status_by_bitfield( fd, &bitfield );

    if ( result == BATON_SUCCESS )
    {
        printf( "Status: %04X\n", bitfield );
    }


    return result;
}
