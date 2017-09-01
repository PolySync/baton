# language: en

Feature: Internal - write command out to the relay

    Writing command out to the relay

    Scenario: Invalid command provided to function
        Given the function is called with null pointer "command" argument
        Then the function should return an error

    Scenario: Function called during function execution returned error
        Given a called function returns an error
        Then the function should return an error
