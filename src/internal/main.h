/**
 * @file internal/main.h
 * @brief Internal command line interface functions
 */


#ifndef _INTERNAL_BATON_H_
#define _INTERNAL_BATON_H_


#include "baton.h"


/* For testing, yuck_t must be mocked as args.h is not compilable by the
C++ compiler used for tests */
#ifdef __cplusplus
typedef int yuck_t;
#endif


baton_result_t parse_enable_cmd(
    int const fd,
    yuck_t const * const argp );

baton_result_t parse_disable_cmd(
    int const fd,
    yuck_t const * const argp );

baton_result_t parse_read_cmd(
    int const fd,
    yuck_t const * const argp );

baton_result_t parse_set_id_cmd(
    int const fd,
    yuck_t const * const argp );

baton_result_t parse_get_id_cmd(
    int const fd );

baton_result_t parse_get_firmware_version_cmd(
    int const fd );

baton_result_t parse_toggle_bitfield_cmd(
    int const fd,
    yuck_t const * const argp );

baton_result_t parse_read_bitfield_cmd(
    int const fd );

baton_result_t check_relay_argument(
    int * const relay_num,
    yuck_t const * const argp );

baton_result_t check_id_argument(
    char * const id,
    yuck_t const * const argp );

baton_result_t check_bitfield_argument(
    unsigned long * const bitfield,
    yuck_t const * const argp );


#endif /* _INTERNAL_BATON_H_ */
