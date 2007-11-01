The LibFakeTime library adds filter to function time().

Dynamically linked programs which checking time using time() or gettimeofday()
will report fake time, so it is possible to send e-mail to friend with
date of his birth after this date if we forgot. ;-P

You can also use it to warp time in xclock. Or to make some shareware program
works after expiration date (but this is obviously illegal).

For more information (about usage of library) see manual (libfaketime.8).

I used some code from:
 - Anubis (GPL): http://www.gnu.org/projects/anubis/,
 - libsafe (LGPL): http://www.bell-labs.com/org/11356/libsafe.html,

To build the library:
        make            (This will make libfaketime.so.1)
        make install    (make install must be done as root)

LibFakeTime library have two configure option (see Makefile),
     -DDEBUG            (This enable _very_ noisy debug code,
        (optional)       not very useful for normal users, just for programmers)
     -DKILLER           (This enable killer code,
        (optional)       if library cannot read fake code from file,
                         it will kill your process. Useful with programs
                         writing timestamps - cannot get fake time,
                         prevent program from make timestamps.
                         DANGER: IT CAN KILL YOUR INIT AND LEAD TO BOOT FAILURE!)

The makefile will only copy the library and man page to the appropriate
directories.  To activate libfaketime, either on a per-process basis or on a
system-wide basis, see the man page (libfaketime.8, Section "Installing
libfaketime").

See http://www.burghardt.pl/ for more information and new releases.
Send comments and bug reports to krzysztof@burghardt.pl.
