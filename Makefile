#
#   Makefile for LibFakeTime library.
#   Copyright (c) 2002-2004 Krzysztof Burghardt <einstein@underground.org.pl>.
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

all: libfaketime.so.0

libfaketime.so.0: libfaketime.o
	$(CC) -shared libfaketime.o -ldl -Wl,-soname -Wl,libfaketime.so.0 -o libfaketime.so.0

libfaketime.o: libfaketime.c libfaketime.h
	$(CC) -O2 -c libfaketime.c -fPIC -DPIC -o libfaketime.o

install:
	install -c -s -m 0755 libfaketime.so.0 /lib
	install -c    -m 0644 libfaketime.8 /usr/share/man/man8/

clean:
	rm -f *.o *~ libfaketime.so*

indent:
	indent *.c *.h
	rm -f *~
