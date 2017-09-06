# Baton

Library and command line interface for the
[Numato 16 Channel USB Relay Module](https://numato.com/product/16-channel-usb-relay-module).


## Getting Started

There is a library for use in other projects as well as a simple command line
interface that exercises all of the functionality.

### Prerequisites

A Linux machine with CMake (version 2.8 or greater) is required:

```
sudo apt install cmake
```

### Building

Using the default CMake configuration:

```
mkdir build
cd build
cmake ..
make
```

This will create a static library (`libbaton.a`) and a shared library (`libbaton.so`),
and a command line interface (`baton`) to directly interact with the module.

## API

**Open file descriptor associated with the module's serial device.**

```c
baton_result_t baton_open(
    char const * const port,
    int * const fd )
```

Opens a serial port (e.g., `/dev/ttyACM0`) and returns the opened file descriptor
in `fd`.


**Close the previously opened file descriptor.**

```c
baton_result_t baton_close(
    int const fd )
```

Closes the file descriptor previously opened by `baton_open()`.


**Get the relay module's firmware version.**

```c
baton_result_t baton_get_firmware_version(
    int const fd,
    char * const firmware,
    int const firmware_length )
```

Queries the relay module for its firmware version and returns it inside of
`firmware`.


**Set the relay module's module ID.**

```c
baton_result_t baton_set_id(
    int const fd,
    char const * const id,
    int const id_length )
```

Commands the relay module to set its ID to `id`.


**Get the relay module's current module ID.**

```c
baton_result_t baton_get_id(
    int const fd,
    char * const id,
    int const id_length )
```

Queries the relay module for its current ID and returns it inside of `id`.


**Get the status of a specific relay.**
```c
baton_result_t baton_get_relay_status(
    int const fd,
    unsigned int const relay,
    baton_relay_status_t * status )
```

Queries the relay module for the status of relay number `relay` and returns it
in `status`.


**Get the status of all relays.**

```c
baton_result_t baton_get_relay_status_by_bitfield(
    int const fd,
    unsigned long * bitfield )
```

Queries the relay module for the status of all relays and returns it in `bitfield`
as a bitfield where a `1` in bit position N means relay N is enabled.


**Enable a specific relay.**

```c
baton_result_t baton_enable_relay(
    int const fd,
    unsigned int const relay )
```

Commands the relay module to enable relay number `relay`.


**Disable a specific relay.**

```c
baton_result_t baton_disable_relay(
    int const fd,
    unsigned int const relay )
```

Commands the relay module to disable relay number `relay`.


**Toggle multiple relays with a bitfield.**

```c
baton_result_t baton_toggle_relays_by_bitfield(
    int const fd,
    unsigned long const bitfield )
```

Commands the relay module to set the status of all relays with a bitfield where
a `1` in bit position N means relay N is enabled.


## CLI

The relays use a serial interface to talk to a host machine. After being connected
to a USB port, they'll populate a device in `/dev`. For example, on Ubuntu they
default to `/dev/ttyACMX`, where `X` is a digit starting at `0` and incrementing
by one with each additional relay module that is connected.

This USB serial device must be given to the CLI as an argument in the form of
`-d DEVICE`, where `DEVICE` is something like `/dev/ttyACM0`.

In addition to the serial device, the CLI must also be given one of the following
commands:

```
  enable <RELAY>
     Enable relay number RELAY

  disable <RELAY>
     Disable relay number RELAY

  read <RELAY>
     Get current relay status of relay number RELAY

  set_id <ID>
     Set module ID to ID

  get_id
     Get current module ID

  get_firmware_version
     Get module's firmware version

  toggle_bitfield <BITFIELD>
     Toggle multiple relays with 4-bit BITFIELD

  read_bitfield
     Get current relay status of multiple relays as 4-bit bitfield
```

As an example, if the relay module is connected as serial device `/dev/ttyACM0`,
to turn on Relay 0:

```
./baton -d /dev/ttyACM0 enable 0
```


## Running the Tests

The tests require Cucumber (version 2.0.0), Cucumber-CPP, and Cgreen. Pre-built
libraries of Cucumber-CPP and Cgreen are included in `tests/framework`, but
Cucumber must be installed:

```
sudo apt install ruby-dev libboost-dev
sudo gem install cucumber -v 2.0.0
```

You can then run the tests with CMake:

```
mkdir build
cd build
cmake .. -DTESTS=ON
make run-unit-tests
```


## Thanks
* [Yuck](https://github.com/hroptatyr/yuck) was used for CLI arg parsing
