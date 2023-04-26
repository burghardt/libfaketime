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

LIBEXT = so.1

CFLAGS += -DVERSION="20230219"
CFLAGS += -DDEBUG
#CFLAGS += -DKILLER

CFLAGS += -O2
CFLAGS += -Wall -Werror -Wextra -Wdate-time
CFLAGS += -Wformat -Wformat-security -D_FORTIFY_SOURCE=2
CFLAGS += -Wcast-align -Wcast-qual -Wchar-subscripts
CFLAGS += -Wformat-nonliteral -Wpointer-arith -Wredundant-decls
CFLAGS += -Wreturn-type -Wshadow -Wswitch -Wunused-parameter -Wwrite-strings
CFLAGS += -fstack-protector-strong -fPIC -DPIC

OS := $(shell uname -s)

ifeq ($(OS), Linux)
	LDFLAGS += -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack
	LDFLAGS += -Wl,--as-needed -Wl,-Bsymbolic-functions -Wl,--fatal-warnings
	LDFLAGS += -fPIC -ldl -Wl,-soname -Wl,libfaketime.$(LIBEXT)

	LIBDIRSUFFIX = $(test `uname -m` = x86_64 && echo 64)
endif

ifeq ($(OS), Darwin)
	LIBEXT = dylib
endif

all: libfaketime.$(LIBEXT)

libfaketime.$(LIBEXT): libfaketime.o
	$(CC) $(LDFLAGS) -shared libfaketime.o -o libfaketime.$(LIBEXT)

libfaketime.o: libfaketime.c libfaketime.h
	$(CC) $(CFLAGS) -c libfaketime.c -o libfaketime.o

install:
	install -d -m 0755 $(DESTDIR)/lib$(LIBDIRSUFFIX)
	install -c -m 0755 libfaketime.$(LIBEXT) $(DESTDIR)/lib$(LIBDIRSUFFIX)
	install -d -m 0755 $(DESTDIR)/usr/share/man/man8
	install -c -m 0644 libfaketime.8 $(DESTDIR)/usr/share/man/man8

clean:
	rm -f *.o *~ libfaketime.$(LIBEXT)

indent:
	indent *.c *.h
	rm -f *~
