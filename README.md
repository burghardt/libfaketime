# libFakeTime

LibFakeTime - a library that adds a filter to time() function calls

## Description

Dynamically linked programs that check the time using the `time()`
or `gettimeofday()` functions would report fake time to make it possible
to send an e-mail to a friend with the date of his birthday after this
date has already passed if one forgot.

You can also use it to warp time in `xclock` or to test programs dealing
with expiration dates (i.e., shareware programs work forever,
SSL certificates never expire, and debugged programs start each time
with the same time settings).

For more information about library usage, refer to the manual
([libfaketime.8](libfaketime.8)).

## Getting Started

These instructions will get you a copy of the project up and running on your
local machine.

### Downloading

Simply clone repository.

    git clone https://github.com/burghardt/libfaketime.git

### Building from sources

Build sources with `make` to create binary `libfaketime.so.1`.

    make

#### Build options

LibFakeTime library has two optional build-time configuration options
(see [Makefile](Makefile)).

 * `-DDEBUG` (optional) This enables verbose debug code. Not very useful
             for regular usage, but it might be helpful for troubleshooting.

 * `-DKILLER` (optional) This enables the killer code. If the library cannot
              read fake time from the file, it will kill the process. Helpful
              with programs writing timestamps or invalidating something after
              the expiration date.

Warning: `-DKILLER` option can kill your `/sbin/init` early during boot process
rendering system unable to boot if enabled system-wide!

### Installing

Run `make install` (as root) to install binary library and manual page.

    sudo make install

The `Makefile` install target will only copy the library and man page to the
appropriate directories. To activate libfaketime, either on a per-process
basis or a system-wide basis, see the section "Installing libfaketime"
on the manual page.

## Usage

Configure fake timestamp, preload library and run target program.

    mkdir -p  ~/.libfaketime.d
    date -d 20200101 +%s > ~/.libfaketime.d/date.fake
    env LD_PRELOAD=/lib/libfaketime.so.1 date -R # prints "Wed, 01 Jan 2020  00:00:00 +0100"

## Acknowledgments

I used some code from:
 - Anubis (GPL): http://www.gnu.org/projects/anubis/,
 - libsafe (LGPL): http://www.bell-labs.com/org/11356/libsafe.html,

## Footnote

See http://github.com/burghardt/libfaketime for more information and new
releases. Send comments and bug reports to <krzysztof@burghardt.pl>.
