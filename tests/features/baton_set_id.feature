# language: en

Feature: Set module ID

    Setting ID of the relay module

    Scenario: Invalid ID provided to function
        Given the function is called with invalid "id" argument
        Then the function should return an error

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function        |
            | write_command() |

    Scenario: Function completes successfully
        Given the function completes without error
        Then the function should return success
