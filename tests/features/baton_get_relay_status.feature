# language: en

Feature: Get relay status

    Getting status of a specific relay

    Scenario: Invalid receive buffer provided to function
        Given the function is called with null pointer "status" argument
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
