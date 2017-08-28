/**
 * @file internal/baton.h
 * @brief Internal Baton library functions
 */


 #ifndef _INTERNAL_BATON_H_
 #define _INTERNAL_BATON_H_


 #include "baton.h"


 baton_result_t write_command(
    int const fd,
    char const * const command,
    int const length );

baton_result_t read_response(
    int const fd,
    char * const response,
    int const length );


#endif /* _INTERNAL_BATON_H_ */
