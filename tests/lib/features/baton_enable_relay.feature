# language: en

Feature: Enable a specific relay

    Enabling a specific relay

    Scenario Outline: Function called during function execution returned error
        Given <function> returns an error
        Then the function should return an error

        Examples:
            | function                 |
            | baton_get_relay_status() |
            | write_command()          |

    Scenario: Function completes successfully
        Given the function completes without error
        Then the function should return success

    Scenario: Enable requested on already enabled relay
        Given a relay is currently enabled
        Then the function should ignore the request
