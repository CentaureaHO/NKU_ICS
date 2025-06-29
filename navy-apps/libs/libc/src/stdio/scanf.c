/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "local.h"
#include <_ansi.h>
#include <stdio.h>

#ifdef _HAVE_STDC
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#ifndef _REENT_ONLY

int
#ifdef _HAVE_STDC
scanf (const char *fmt, ...)
#else
<<<<<<< HEAD
    scanf(fmt, va_alist) char* fmt;
=======
    scanf(fmt, va_alist) char *fmt;
>>>>>>> master
va_dcl
#endif
{
    int     ret;
    va_list ap;

#ifdef _HAVE_STDC
<<<<<<< HEAD
    va_start(ap, fmt);
#else
    va_start(ap);
#endif
    ret = __svfscanf(_stdin_r(_REENT), fmt, ap);
    va_end(ap);
    return ret;
=======
  va_start(ap, fmt);
#else
  va_start(ap);
#endif
  ret = __svfscanf(_stdin_r(_REENT), fmt, ap);
  va_end(ap);
  return ret;
>>>>>>> master
}

#endif

int
#ifdef _HAVE_STDC
_scanf_r (struct _reent *ptr, const char *fmt, ...)
#else
<<<<<<< HEAD
    _scanf_r(ptr, fmt, va_alist) struct _reent* ptr;
char* fmt;
=======
    _scanf_r(ptr, fmt, va_alist) struct _reent *ptr;
char *fmt;
>>>>>>> master
va_dcl
#endif
{
    int     ret;
    va_list ap;

#ifdef _HAVE_STDC
<<<<<<< HEAD
    va_start(ap, fmt);
#else
    va_start(ap);
#endif
    ret = __svfscanf(_stdin_r(ptr), fmt, ap);
    va_end(ap);
    return (ret);
=======
  va_start(ap, fmt);
#else
  va_start(ap);
#endif
  ret = __svfscanf(_stdin_r(ptr), fmt, ap);
  va_end(ap);
  return (ret);
>>>>>>> master
}
