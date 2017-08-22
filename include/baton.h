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
 * @return BATON_SUCCESS - Serial interface successfully initialized.
 *         BATON_ERROR - Serial interface unsuccessfully initialized due to error.
 *
 */
baton_result_t baton_init(
    char const * const port,
    int const speed,
    int * const fd );


/**
 * @brief Get the firmware version of the relay module.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [out] rx_buf - The buffer in which to return the response.
 *        [in] rx_buf_length - The length of rx_buf.
 *
 * @return BATON_SUCCESS - Query was successful and rx_buf contains a valid response.
 *         BATON_ERROR - Query was unsuccessful and rx_buf does not contain a valid response.
 *
 */
baton_result_t baton_get_firmware_version(
    int const fd,
    char * const rx_buf,
    int const rx_buf_length );
