# language: en

Feature: Get firmware version

    Getting firmware version of the relay module

    Scenario: Invalid receive buffer provided to function
        Given the function is called with null pointer "rx_buf" argument
        Then the function should return an error

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function        |
            | write_command() |
            | read_response() |

    Scenario: Function completes successfully
        Given the function completes without error
        Then the function should return success
