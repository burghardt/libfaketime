#
#   Makefile for LibFakeTime library
#   Copyright (C) 2002-2004,2023 Krzysztof Burghardt <krzysztof@burghardt.pl>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

all: libfaketime.so.1

CFLAGS = -DVERSION="20071101"
# -DDEBUG
# -DKILLER

libfaketime.so.1: libfaketime.o
	$(CC) -shared libfaketime.o -fPIC -ldl -Wl,-soname -Wl,libfaketime.so.1 -o libfaketime.so.1

libfaketime.o: libfaketime.c libfaketime.h
	$(CC) $(CFLAGS) -Wall -O2 -c libfaketime.c -fPIC -DPIC -o libfaketime.o

install:
	install -c -s -m 0755 libfaketime.so.1 /lib
	install -c    -m 0644 libfaketime.8 /usr/share/man/man8/

clean:
	rm -f *.o *~ libfaketime.so*

indent:
	indent *.c *.h
	rm -f *~
