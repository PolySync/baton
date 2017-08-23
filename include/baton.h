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

    /* Result of the operation was unsuccessful due to an error. */
    BATON_ERROR,

    /* The operation was ignored because the requested action would do nothing
        (e.g.,  enabling a relay that is already enabled). */
    BATON_IGNORED
} baton_result_t;


/**
 * @brief Enumeration of possible relay statuses.
 *
 */
 typedef enum {
    /* Relay is on. */
    BATON_RELAY_ON,

    /* Relay is off. */
    BATON_RELAY_OFF
} baton_relay_status_t;


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


/**
 * @brief Set the ID of the relay module.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [in] id - ID to set.
 *
 * @return BATON_SUCCESS - Setting the ID was successful.
 *         BATON_ERROR - Setting the ID was not successful.
 *
 */
baton_result_t baton_set_id(
    int const fd,
    unsigned long const id );


/**
 * @brief Get the ID of the relay module.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [out] rx_buf - The buffer in which to return the response.
 *        [in] rx_buf_length - The length of rx_buf.
 *
 * @return BATON_SUCCESS - Query was successful and rx_buf contains a valid response.
 *         BATON_ERROR - Query was unsuccessful and rx_buf does not contain a valid response.
 *
 */
baton_result_t baton_get_id(
    int const fd,
    char * const rx_buf,
    int const rx_buf_length );


/**
 * @brief Get the status of a specific relay.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [in] relay - Number of relay to query.
 *        [out] status - The status of the relay.
 *
 * @return BATON_SUCCESS - Query was successful and rx_buf contains a valid response.
 *         BATON_ERROR - Query was unsuccessful and rx_buf does not contain a valid response.
 *
 */
baton_result_t baton_get_relay_status(
    int const fd,
    unsigned int const relay,
    baton_relay_status_t * status );


/**
 * @brief Get the status of all relays.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [out] bitfield - Bitfield representing statuses of the relays where a
 *                       1 in bit position N means relay N is enabled and a
 *                       0 in bit position N means relay N is disabled.
 *
 * @return BATON_SUCCESS - Query was successful and rx_buf contains a valid response.
 *         BATON_ERROR - Query was unsuccessful and rx_buf does not contain a valid response.
 *
 */
 baton_result_t baton_get_relay_status_by_bitfield(
    int const fd,
    unsigned long * bitfield );


/**
 * @brief Enable a specific relay.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [in] relay - Number of relay to enable.
 *
 * @return BATON_SUCCESS - Enabling the relay was successful.
 *         BATON_ERROR - Enabling the relay was unsuccessful.
 *         BATON_IGNORED - The relay was not enabled because it is already enabled.
 *
 */
baton_result_t baton_enable_relay(
    int const fd,
    unsigned int const relay );


/**
 * @brief Disable a specific relay.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [in] relay - Number of relay to disable.
 *
 * @return BATON_SUCCESS - Enabling the relay was successful.
 *         BATON_ERROR - Enabling the relay was unsuccessful.
 *         BATON_IGNORED - The relay was not disabled because it is already disabled.
 *
 */
baton_result_t baton_disable_relay(
    int const fd,
    unsigned int const relay );


/**
 * @brief Toggle relays on and off using a bitfield.
 *
 * @param [in] fd - The file descriptor of the serial interface.
 *        [in] bitfield - Bitfield representing the relays, where a 1 in
 *                        bit position N enables relay N and a 0 in bit position
 *                        N disables relay N.
 *
 * @return BATON_SUCCESS - Toggling the relays was successful.
 *         BATON_ERROR - Toggling the relays was unsuccessful.
 *
 */
 baton_result_t baton_toggle_relays_by_bitfield(
    int const fd,
    unsigned int const bitfield );
