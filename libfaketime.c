/*
   LibFakeTime library - fake time() library for GNU/Linux.
   Copyright (c) 2002-2004 Krzysztof Burghardt <einstein@underground.org.pl>.
   
   $Id: libfaketime.c,v 1.2 2004-09-21 11:02:03 kb Exp $

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "libfaketime.h"

static void _libfaketime_init () __attribute__ ((constructor));
static void _libfaketime_fini () __attribute__ ((destructor));
static void _libfaketime_init (void);
static void _libfaketime_fini (void);

extern char **environ[];

static int init_done = FALSE;	/* return 1 if _init was run, or 0 otherwise */
static int real_time = TRUE;	/* return real time if 1, or fake if 0 */
static time_t fake_time = 0;	/* fake time readed from file */
void *lib_handle;		/* system library handle */
time_t (*real_time_handle) (time_t *) = NULL;	/* system time() handle */
int (*real_gettimeofday_handle) (struct timeval * tv, struct timezone * tz) = NULL;	/* system gettimeofday() handle */

static void *
xmalloc (int n)
{
  void *p;

  p = (void *) malloc (n);

  if (!p)
    {
      DEBUG_MESSAGE
	("LibFakeTime.SO: xmalloc(): cannot alocate %i bytes...\n", n);
#ifdef KILLER
      /*
       * Should it kill proces?!
       * If proces have SIGSEGV (11) handled it can persist exeption...
       * (if not - glibc will kill it...)
       */
      DEBUG_MESSAGE ("LibFakeTime.SO: xmalloc(): calling _exit()...\n");
      _exit (EXIT_FAILURE);
#endif
    }

  memset (p, 0, n);

  return p;
}

static char *
mk_path (char *path, char *dir, char *file, char *surfix)
{
  sprintf (path, "%s/%s.%s", dir, basename (file), surfix);

  DEBUG_MESSAGE
    ("LibFakeTime.SO: mk_path(): dir=%s, file=%s, surfix=%s\n",
     dir, file, surfix);
  DEBUG_MESSAGE ("LibFakeTime.SO: mk_path(): returning %s\n", path);

  return path;
}

static char *
get_exe (char *exe, int size)
{
  char *temp;
  temp = xmalloc (size);

  if (readlink ("/proc/self/exe", temp, size) <= 0)
    {
      exe[0] = '\0';
      DEBUG_MESSAGE ("LibFakeTime.SO: get_exe(): cannot get exe name...\n");
#ifdef KILLER
      /*
       * It`s impossible?!
       * If you have /proc/ NOT mounted it can kill your proces...
       * (e.g. init while booting and proc is not mounted yet)
       */
      DEBUG_MESSAGE ("LibFakeTime.SO: get_exe(): calling _exit()...\n");
      free (temp);
      temp = NULL;
      _exit (EXIT_FAILURE);
#endif
    }
  else
    {
      sprintf (exe, "%s", basename (temp));
      DEBUG_MESSAGE ("LibFakeTime.SO: get_exe(): returning %s\n", exe);
    }
  free (temp);
  temp = NULL;

  return exe;
}

static int
can_i_fake_test (char *dir, char *exe)
{
  FILE *fd;
  char *path;
  int ret = FALSE;

  path = xmalloc (1024);

  fd = fopen (mk_path (path, dir, exe, NO_FAKE_SURFIX), "r");
  if (!fd)
    {
      DEBUG_MESSAGE
	("LibFakeTime.SO: can_i_fake_test(): cannot open %s\n", path);
      DEBUG_MESSAGE ("LibFakeTime.SO: can_i_fake_test(): returning TRUE\n");
      ret = TRUE;
    }
  else
    {
      DEBUG_MESSAGE ("LibFakeTime.SO: can_i_fake_test(): opened %s\n", path);
      DEBUG_MESSAGE ("LibFakeTime.SO: can_i_fake_test(): returning FALSE\n");
      fclose (fd);
    }
  free (path);
  path = NULL;

  return ret;
}

static time_t
get_fake_time (char *dir, char *exe)
{
  FILE *fd;
  time_t fake_time = 0;
  static const int buf_len = 1024;
  char *buf;
  char *path;

  buf = xmalloc (buf_len);
  path = xmalloc (1024);

  fd = fopen (mk_path (path, dir, exe, FAKE_SURFIX), "r");
  if (!fd)
    {
      fake_time = 0;
      DEBUG_MESSAGE
	("LibFakeTime.SO: get_fake_time(): cannot open %s\n", path);
    }
  else
    {
      fgets (buf, buf_len - 1, fd);
      buf[strlen (buf) - 1] = '\0';
      fake_time = (time_t) atoi (buf);
      fclose (fd);
      DEBUG_MESSAGE ("LibFakeTime.SO: get_fake_time(): opened %s\n", path);
      DEBUG_MESSAGE ("LibFakeTime.SO: get_fake_time(): readed %s\n", buf);
    }
  free (buf);
  buf = NULL;
  free (path);
  path = NULL;

  DEBUG_MESSAGE
    ("LibFakeTime.SO: get_fake_time(): returning %i\n",
     (unsigned int) fake_time);
  return fake_time;
}

time_t
time (time_t * t)
{
  static time_t time_to_return = 0;

  /* Check if we are  already after _init and do _init if not yet */
  if (!init_done)
    _libfaketime_init ();

  if (real_time)
    {
      time_to_return = (*real_time_handle) (NULL);
      DEBUG_MESSAGE
	("LibFakeTime.SO: time(): returning REAL time (%i)\n",
	 (unsigned int) time_to_return);
    }
  else
    {
      time_to_return = (*real_time_handle) (NULL) - fake_time;
      DEBUG_MESSAGE
	("LibFakeTime.SO: time(): returning FAKE time (%i)\n",
	 (unsigned int) time_to_return);
    }

  if (t != 0)
    {
      DEBUG_MESSAGE ("LibFakeTime.SO: time(): setting *t...\n");
      *t = time_to_return;
    }
  return time_to_return;
}

int
gettimeofday (struct timeval *tv, struct timezone *tz)
{
  static int retval = 0;

  /* Check if we are  already after _init and do _init if not yet */
  if (!init_done)
    _libfaketime_init ();

  /* Please do not ask me why... ask glibc's3 folks... */
  if (tv == NULL)
    {
      DEBUG_MESSAGE
	("LibFakeTime.SO: gettimeofday(): returning due to NULL pointers...\n");
      return 0;
    }
  retval = (*real_gettimeofday_handle) (tv, tz);
  DEBUG_MESSAGE ("LibFakeTime.SO: gettimeofday(): retval = %i...\n", retval);
  if (!retval)
    {
      /* do not touch tv_usec - it`s extremaly important to return
         _VALID_ tv_usec due to many programs uses this for hi-res timing
         only tv_sec can be (safely) affected */
      DEBUG_MESSAGE
	("LibFakeTime.SO: gettimeofday(): setting *tv->tv_sec...\n");
      tv->tv_sec = time (NULL);
    }
  DEBUG_MESSAGE ("LibFakeTime.SO: gettimeofday(): returning retval...\n");
  return retval;
}

static void _libfaketime_init () __attribute__ ((constructor));
static void _libfaketime_fini () __attribute__ ((destructor));

static void
_libfaketime_init (void)
{
  time_t time_to_return = 0;
  const static int exe_len = 1024;
  char *error;
  char *exe;

  DEBUG_MESSAGE
    ("LibFakeTime.SO: _init(): initalizing library (%s)...\n", VERSION);

  exe = xmalloc (exe_len);

  lib_handle = dlopen ("/lib/libc.so.6", RTLD_LAZY);
  if (!lib_handle)
    {
      fprintf (stderr, "%s\n", dlerror ());
      exit (1);
    }

  real_time_handle = dlsym (lib_handle, "time");
  if ((error = dlerror ()) != NULL)
    {
      fprintf (stderr, "%s\n", error);
      exit (1);
    }

  real_gettimeofday_handle = dlsym (lib_handle, "gettimeofday");
  if ((error = dlerror ()) != NULL)
    {
      fprintf (stderr, "%s\n", error);
      exit (1);
    }

  get_exe (exe, exe_len);

  if ((real_time = can_i_fake_test (SYS_FAKE_TIME_DIR, exe)))
    {
      char *user_path;
      user_path = xmalloc (1024);

      sprintf (user_path, "%s/%s", getenv ("HOME"), USER_FAKE_TIME_DIR);
      time_to_return = get_fake_time (user_path, exe);
      free (user_path);
      user_path = NULL;

      if (!time_to_return)
	{
	  time_to_return = get_fake_time (SYS_FAKE_TIME_DIR, exe);
	}
    }

  if (time_to_return)
    {
      real_time = FALSE;
#ifdef HAVE_SYSLOG_H
      openlog ("LibFakeTime", LOG_CONS | LOG_NDELAY, LOG_USER);
      syslog (LOG_WARNING,
	      "using FAKE time() for (%s:%i) UID(%i) EUID(%i)",
	      exe, getpid (), getuid (), geteuid ());
      closelog ();
#endif
      fake_time = (*real_time_handle) (NULL) - time_to_return;
    }
  else
    {
      real_time = TRUE;
    }
  free (exe);
  exe = NULL;

  init_done = TRUE;

  return;
}

static void
_libfaketime_fini (void)
{
  DEBUG_MESSAGE ("LibFakeTime.SO: _fini(): finalizing library...\n");
  dlclose (lib_handle);

  return;
}