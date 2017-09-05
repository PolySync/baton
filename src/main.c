/**
 * @file main.c
 * @brief Baton command line interface.
 */


#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include "baton.h"
#include "internal/args.h"
#include "internal/main.h"


/* For testing, main() must be renamed to not conflict with the main() running
in the test library */
#ifdef TESTS
#define MAIN test_main
#else
#define MAIN main
#endif


int MAIN(
    int argc,
    char ** argv)
{
    baton_result_t result = BATON_ERROR;
    yuck_t argp[1];
    int fd;


    yuck_parse(argp, argc, argv);


    if ( argp->device_arg == NULL )
    {
        yuck_auto_help( argp );

        fprintf( stderr, "ERROR: DEVICE is a mandatory argument\n\n" );
    }
    else if ( argp->cmd == BATON_CMD_NONE )
    {
        yuck_auto_help( argp );

        fprintf( stderr, "ERROR: COMMAND is a mandatory argument\n\n" );
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
            result = parse_get_id_cmd( fd );
        }
        else if ( argp->cmd == BATON_CMD_GET_FIRMWARE_VERSION )
        {
            result = parse_get_firmware_version_cmd( fd );
        }
        else if ( argp->cmd == BATON_CMD_TOGGLE_BITFIELD )
        {
            result = parse_toggle_bitfield_cmd( fd, argp );
        }
        else if ( argp->cmd == BATON_CMD_READ_BITFIELD )
        {
            result = parse_read_bitfield_cmd( fd );
        }

        baton_close( fd );
    }


    yuck_free(argp);


    return (int)result;
}




/* Internal */
baton_result_t parse_enable_cmd(
    int const fd,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    int relay_num;


    result = check_relay_argument( &relay_num, argp );

    if ( result == BATON_SUCCESS )
    {
        result = baton_enable_relay( fd, relay_num );
    }


    return result;
}


baton_result_t parse_disable_cmd(
    int const fd,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    int relay_num;


    result = check_relay_argument( &relay_num, argp );

    if ( result == BATON_SUCCESS )
    {
        result = baton_disable_relay( fd, relay_num );
    }


    return result;
}


baton_result_t parse_read_cmd(
    int const fd,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    int relay_num;
    baton_relay_status_t status;


    result = check_relay_argument( &relay_num, argp );

    if ( result == BATON_SUCCESS )
    {
        result = baton_get_relay_status( fd, relay_num, &status );
    }


    if ( result == BATON_SUCCESS )
    {
        printf( "Relay %d: ", relay_num );

        if ( status == BATON_RELAY_ON )
        {
            printf( "ON\n" );
        }
        else
        {
            printf( "OFF\n" );
        }
    }


    return result;
}


baton_result_t parse_set_id_cmd(
    int const fd,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    char id[MODULE_ID_LENGTH];


    result = check_id_argument( id, argp );

    if ( result == BATON_SUCCESS )
    {
        result = baton_set_id( fd, id, strlen(id) );
    }


    return result;
}


baton_result_t parse_get_id_cmd(
    int const fd )
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


baton_result_t parse_get_firmware_version_cmd(
    int const fd )
{
    baton_result_t result = BATON_ERROR;
    char version[9];


    result = baton_get_firmware_version( fd, version, sizeof(version) );

    if ( result == BATON_SUCCESS )
    {
        printf( "Firmware: %s\n", version );
    }


    return result;
}


baton_result_t parse_toggle_bitfield_cmd(
    int const fd,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    unsigned long bitfield;


    result = check_bitfield_argument( &bitfield, argp );

    if ( result == BATON_SUCCESS )
    {
        result = baton_toggle_relays_by_bitfield( fd, bitfield );
    }


    return result;
}


baton_result_t parse_read_bitfield_cmd(
    int const fd )
{
    baton_result_t result = BATON_ERROR;
    unsigned long bitfield;


    result = baton_get_relay_status_by_bitfield( fd, &bitfield );

    if ( result == BATON_SUCCESS )
    {
        printf( "Status: %04lX\n", bitfield );
    }


    return result;
}


baton_result_t check_relay_argument(
    int * const relay_num,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    int relay_num_local;
    char * relay_arg_string = NULL;


    if ( argp->nargs == 1 )
    {
        result = BATON_SUCCESS;
    }


    if ( result == BATON_SUCCESS )
    {
        relay_arg_string = argp->args[0];
        size_t relay_arg_string_len = strlen( relay_arg_string );

        size_t i = 0;

        for ( i = 0; i < relay_arg_string_len; ++i )
        {
            int ret = isdigit( relay_arg_string[i] );

            if ( ret == 0 )
            {
                result = BATON_ERROR;
            }
        }
    }


    if ( result == BATON_SUCCESS )
    {
        relay_num_local = atoi( relay_arg_string );

        if ( (relay_num_local < 0)
            || (relay_num_local > (RELAY_COUNT-1)) )
        {
            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        *relay_num = relay_num_local;
    }
    else
    {
        yuck_auto_help( argp );

        fprintf(
            stderr,
            "ERROR: Invalid arguments - RELAY must be between 0 and %d\n",
            (RELAY_COUNT-1) );
    }


    return result;
}


baton_result_t check_id_argument(
    char * const id,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    char * id_arg_string;
    size_t id_arg_string_len;


    if ( argp->nargs == 1 )
    {
        result = BATON_SUCCESS;
    }


    if ( result == BATON_SUCCESS )
    {
        id_arg_string = argp->args[0];
        id_arg_string_len = strlen( id_arg_string );

        if ( id_arg_string_len != MODULE_ID_LENGTH )
        {
            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        size_t i = 0;

        for ( i = 0; i < id_arg_string_len; ++i )
        {
            int ret = isgraph( id_arg_string[i] );

            if ( ret == 0 )
            {
                result = BATON_ERROR;
            }
        }
    }


    if ( result == BATON_SUCCESS )
    {
        strncpy(id, id_arg_string, id_arg_string_len );
    }
    else
    {
        yuck_auto_help( argp );

        fprintf(
            stderr,
            "ERROR: Invalid arguments - ID must be %d printable characters\n",
            MODULE_ID_LENGTH );
    }


    return result;
}


baton_result_t check_bitfield_argument(
    unsigned long * const bitfield,
    yuck_t const * const argp )
{
    baton_result_t result = BATON_ERROR;
    unsigned long bitfield_local;
    char * bitfield_arg_string = NULL;
    size_t bitfield_arg_string_len = -1;


    if ( argp->nargs == 1 )
    {
        result = BATON_SUCCESS;
    }


    if ( result == BATON_SUCCESS )
    {
        bitfield_arg_string = argp->args[0];
        bitfield_arg_string_len = strlen( bitfield_arg_string );

        if ( bitfield_arg_string_len != RELAY_BITFIELD_LENGTH )
        {
            result = BATON_ERROR;
        }
    }


    if ( result == BATON_SUCCESS )
    {
        size_t i = 0;

        for ( i = 0; i < bitfield_arg_string_len; ++i )
        {
            int ret = isxdigit( bitfield_arg_string[i] );

            if ( ret == 0 )
            {
                result = BATON_ERROR;
            }
        }
    }


    if ( result == BATON_SUCCESS )
    {
        bitfield_local = strtoul( argp->args[0], NULL, 16 );

        *bitfield = bitfield_local;
    }
    else
    {
        yuck_auto_help( argp );

        fprintf(
            stderr,
            "ERROR: Invalid arguments - BITFIELD must be between %04X and %04X\n",
            RELAY_BITFIELD_SIZE_MIN,
            RELAY_BITFIELD_SIZE_MAX );
    }


    return result;
}
