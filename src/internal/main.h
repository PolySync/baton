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
    size_t const arg_count,
    char const * args[] );

baton_result_t parse_disable_cmd(
    int const fd,
    size_t const arg_count,
    char const * args[] );

baton_result_t parse_read_cmd(
    int const fd,
    size_t const arg_count,
    char const * args[] );

baton_result_t parse_set_id_cmd(
    int const fd,
    size_t const arg_count,
    char const * args[] );

baton_result_t parse_get_id_cmd(
    int const fd );

baton_result_t parse_get_firmware_version_cmd(
    int const fd );

baton_result_t parse_toggle_bitfield_cmd(
    int const fd,
    size_t const arg_count,
    char const * args[] );

baton_result_t parse_read_bitfield_cmd(
    int const fd );

baton_result_t check_relay_argument(
    int * const relay_num,
    char const * const relay_arg );

baton_result_t check_id_argument(
    char * const id,
    char const * const id_arg );

baton_result_t check_bitfield_argument(
    unsigned long * const bitfield,
    char const * const bitfield_arg );


#endif /* _INTERNAL_BATON_H_ */
