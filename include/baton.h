/**
 * @file baton.h
 * @brief Baton library interface.
 */


/**
 * @brief Enumeration of possible error values.
 *
 */
typedef enum {
    /* Result of the operation was successful. */
    BATON_SUCCESS,

    /* Result of the operaton was unsuccessful due to an error. */
    BATON_ERROR
} baton_result_t;


/**
 * @brief Initialize the serial interface settings.
 *
 * @param [in] speed - The baud rate to use for the serial interface.
 *        [out] fd - The created file descriptor of the serial interface.
 *
 * @return SUCCESS - Serial interface successfully initialized.
 *         ERROR - Serial interface unsuccessfully initialized due to error.
 *
 */
baton_result_t baton_init(
    char const * const port,
    int const speed,
    int * const fd );
