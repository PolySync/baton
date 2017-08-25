# language: en

Feature: Initialization of baton

    Initialization of interface to relay device

    Scenario: Invalid port provided to function
        Given the function is called with null pointer "port" argument
        Then the function should return an error

    Scenario: Invalid file descriptor provided to function
        Given the function is called with null pointer "fd" argument
        Then the function should return an error

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function               |
            | open()                 |
            | tcgetattr()            |
            | cfsetospeed()          |
            | cfsetispeed()          |
            | tcsetattr()            |

    Scenario: Function completes successfully
        Given the function completes without error
        Then the function should return success
