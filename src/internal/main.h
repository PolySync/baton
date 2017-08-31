/**
 * @file internal/main.h
 * @brief Internal command line interface functions
 */


#ifndef _INTERNAL_BATON_H_
#define _INTERNAL_BATON_H_


#include "baton.h"


baton_result_t parse_enable_cmd( int fd, yuck_t *argp );
baton_result_t parse_disable_cmd( int fd, yuck_t *argp );
baton_result_t parse_read_cmd( int fd, yuck_t *argp );
baton_result_t parse_set_id_cmd( int fd, yuck_t *argp );
baton_result_t parse_get_id_cmd( int fd, yuck_t *argp );
baton_result_t parse_get_firmware_version_cmd( int fd, yuck_t *argp );
baton_result_t parse_toggle_bitfield_cmd( int fd, yuck_t *argp );
baton_result_t parse_read_bitfield_cmd( int fd, yuck_t *argp );


#endif /* _INTERNAL_BATON_H_ */
