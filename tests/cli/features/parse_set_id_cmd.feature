# language: en

Feature: Closing of baton serial interface

    Closing of interface to relay device

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return BATON_ERROR

        Examples:
            | function            |
            | check_id_argument() |
            | baton_set_id()      |

    Scenario Outline: Argument validity checks
        Given <arg_count> number of arguments are given
        Then the function should return <return_code>

        Examples:
            | arg_count | return_code   |
            | 0         | BATON_ERROR   |
            | 2         | BATON_ERROR   |
            | 1         | BATON_SUCCESS |
