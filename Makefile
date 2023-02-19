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

CFLAGS = -DVERSION="20230219"
#CFLAGS += -DDEBUG
#CFLAGS += -DKILLER

CFLAGS += -Wall -Werror -Wextra -Wdate-time
CFLAGS += -Wformat -Wformat-security -D_FORTIFY_SOURCE=2
CFLAGS += -Wcast-align -Wcast-qual -Wchar-subscripts
CFLAGS += -Wformat-nonliteral -Wpointer-arith -Wredundant-decls
CFLAGS += -Wreturn-type -Wshadow -Wswitch -Wunused-parameter -Wwrite-strings
CFLAGS += -fstack-protector-strong -fPIC -fPIE

LDFLAGS += -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack
LDFLAGS += -Wl,--as-needed -Wl,-Bsymbolic-functions -Wl,--fatal-warnings

libfaketime.so.1: libfaketime.o
	$(CC) $(LDFLAGS) -shared libfaketime.o -fPIC -ldl -Wl,-soname -Wl,libfaketime.so.1 -o libfaketime.so.1

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
