# language: en

Feature: Internal - read response from the relay

    Readinga  response from the relay

    Scenario: Invalid command provided to function
        Given the function is called with null pointer "response" argument
        Then the function should return an error

    Scenario: Function called during function execution returned error
        Given a called function returns an error
        Then the function should return an error
