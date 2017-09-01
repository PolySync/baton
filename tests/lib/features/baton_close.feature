# language: en

Feature: Closing of baton serial interface

    Closing of interface to relay device

    Scenario: Invalid file descriptor provided to function
        Given the function is called with null pointer "fd" argument
        Then the function should return an error

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function |
            | close()  |

    Scenario: Function completes successfully
        Given the function completes without error
        Then the function should return success
