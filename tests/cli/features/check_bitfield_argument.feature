# language: en

Feature: Closing of baton serial interface

    Closing of interface to relay device

    Scenario Outline: Argument validity checks
        Given argument <bitfield> is given to function
        Then the function should return <return_code>

        Examples:
            | bitfield | return_code   |
            | gggg     | BATON_ERROR   |
            | $!@#     | BATON_ERROR   |
            | 00000000 | BATON_ERROR   |
            | 0000     | BATON_SUCCESS |
            | ffff     | BATON_SUCCESS |
