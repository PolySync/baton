# language: en

Feature: Closing of baton serial interface

    Closing of interface to relay device

    Scenario Outline: Argument validity checks
        Given argument <relay> is given to function
        Then the function should return <return_code>

        Examples:
            | relay | return_code   |
            | -1    | BATON_ERROR   |
            | 16    | BATON_ERROR   |
            | abc   | BATON_ERROR   |
            | $!%   | BATON_ERROR   |
            | 0     | BATON_SUCCESS |
            | 15    | BATON_SUCCESS |
