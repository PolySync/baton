# language: en

Feature: Closing of baton serial interface

    Closing of interface to relay device

    Scenario: No arguments provided to CLI
        Given the CLI is executed with no arguments
        Then the CLI should return an error

    Scenario: DEVICE provided without COMMAND
        Given the CLI is executed with only DEVICE argument
        Then the CLI should return an error

    Scenario: COMMAND provided without DEVICE
        Given the CLI is executed with only COMMAND argument
        Then the CLI should return an error

    Scenario Outline: Function called during CLI execution returned error
        Given <function> returns an error
        Then the CLI should return an error

        Examples:
            | function                         |
            | baton_open()                     |
            | parse_enable_cmd()               |
            | parse_disable_cmd()              |
            | parse_read_cmd()                 |
            | parse_set_id_cmd()               |
            | parse_get_id_cmd()               |
            | parse_get_firmware_version_cmd() |
            | parse_toggle_bitfield_cmd()      |
            | parse_read_bitfield_cmd()        |
