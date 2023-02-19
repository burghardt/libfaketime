# libFakeTime
LibFakeTime - a library that adds a filter to time() function calls

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

I used some code from:
 - Anubis (GPL): http://www.gnu.org/projects/anubis/,
 - libsafe (LGPL): http://www.bell-labs.com/org/11356/libsafe.html,

To build and install library:

    make          # this will make libfaketime.so.1
    make install  # make install must be done as root or with sudo

LibFakeTime library has two configure options (see [Makefile](Makefile)).

    -DDEBUG  # (optional) This enables verbose debug code. Not very useful
               for regular usage, but it might be helpful for troubleshooting.
    -DKILLER # (optional) This enables the killer code. If the library cannot
               read fake time from the file, it will kill the process. Helpful
               with programs writing timestamps or invalidating something after
               the expiration date.

               DANGER: THIS OPTION CAN KILL YOUR INIT AND LEAD TO BOOT FAILURE
                       IF ENABLED SYSTEM-WIDE!

The `Makefile` install target will only copy the library and man page to the
appropriate directories. To activate libfaketime, either on a per-process
basis or a system-wide basis, see the section "Installing libfaketime"
on the manual page.

See http://github.com/burghardt/libfaketime for more information and new
releases. Send comments and bug reports to <krzysztof@burghardt.pl>.
