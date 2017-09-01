# language: en

Feature: Toggle multiple relays with a bitfield

    Toggling multiple relays with a bitfield

    Scenario: Invalid bitfield provided to function
        Given the function is called with invalid "bitfield" argument
        Then the function should return an error

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function                                      |
            | write_command()                               |
            | baton_get_relay_status_by_bitfield()          |

    Scenario: Function completes successfully
        Given the function completes without error
        Then the function should return success
