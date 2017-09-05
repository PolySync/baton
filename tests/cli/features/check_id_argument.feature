# language: en

Feature: Closing of baton serial interface

    Closing of interface to relay device

    Scenario Outline: Argument validity checks
        Given argument <relay> is given to function
        Then the function should return <return_code>

        Examples:
            | relay    | return_code     |
            | 1        | BATON_ERROR   |
            | 22       | BATON_ERROR   |
            | 333      | BATON_ERROR   |
            | 4444     | BATON_ERROR   |
            | 55555    | BATON_ERROR   |
            | 666666   | BATON_ERROR   |
            | 7777777  | BATON_ERROR   |
            | 88888888 | BATON_SUCCESS |
            | abcdefgh | BATON_SUCCESS |
            | !@#$%^&* | BATON_SUCCESS |
