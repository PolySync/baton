# language: en

Feature: Closing of baton serial interface

    Closing of interface to relay device

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function                     |
            | baton_get_firmware_version() |
