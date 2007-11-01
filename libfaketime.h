/* 
   Header for LibFakeTime library
   Copyright (c) 2002-2004 Krzysztof Burghardt <krzysztof@burghardt.pl>
   
   $Id: libfaketime.h,v 1.4 2007-11-01 09:02:34 kb Exp $
 
   This program is free software; you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation; either version 2 of the License, or 
   (at your option) any later version. 
 
   This program is distributed in the hope that it will be useful, 
   but WITHOUT ANY WARRANTY; without even the implied warranty of 
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
   GNU General Public License for more details. 
 
   You should have received a copy of the GNU General Public License 
   along with this program; if not, write to the Free Software 
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef _LIBFAKETIME_H
#define _LIBFAKETIME_H

#include <time.h>		/* time_t */
#include <unistd.h>		/* getpid(), readlink(), _exit() */
#include <stdio.h>		/* sprintf(), fgets()... */
#include <stdlib.h>		/* atoi() */
#include <string.h>		/* memset() */
#include <libgen.h>		/* basename() */
#include <sys/time.h>		/* gettimeofday() */
#define __USE_GNU 1
#include <dlfcn.h>		/* dlsym() */
#include <syslog.h>		/* syslog() */

/* Define system and user configuration directory */
#define SYS_FAKE_TIME_DIR "/etc/libfaketime.d"
#define USER_FAKE_TIME_DIR ".libfaketime.d"

/* No fake and nofake files surfix (without dot!) */
#define NO_FAKE_SURFIX "nofake"
#define FAKE_SURFIX "fake"

/* Define INTeger value for TRUE and FALSE */
#define FALSE 0
#define TRUE  1

/* Define DEBUG code */
#ifdef DEBUG
#define DEBUG_MESSAGE(format, args...) \
   fprintf(stderr, format, ##args)
#else
#define DEBUG_MESSAGE(format, args...)
#endif

#endif
