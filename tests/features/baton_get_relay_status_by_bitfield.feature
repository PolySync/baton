# language: en

Feature: Get relay status by bitfield

    Getting status of multiple relays with a bitfield

    Scenario: Invalid bitfield provided to function
        Given the function is called with null pointer "bitfield" argument
        Then the function should return an error

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function        |
            | write_command() |
            | read_response() |
            | strtoul()       |

    Scenario: Function completes successfully
        Given the function completes without error
        Then the function should return success
