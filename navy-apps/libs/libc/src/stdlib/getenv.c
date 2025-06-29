/*
FUNCTION
<<getenv>>---look up environment variable

INDEX
        getenv
INDEX
        environ

ANSI_SYNOPSIS
        #include <stdlib.h>
        char *getenv(const char *<[name]>);

TRAD_SYNOPSIS
        #include <stdlib.h>
        char *getenv(<[name]>)
        char *<[name]>;

DESCRIPTION
<<getenv>> searches the list of environment variable names and values
(using the global pointer `<<char **environ>>') for a variable whose
name matches the string at <[name]>.  If a variable name matches,
<<getenv>> returns a pointer to the associated value.

RETURNS
A pointer to the (string) value of the environment variable, or
<<NULL>> if there is no such environment variable.

PORTABILITY
<<getenv>> is ANSI, but the rules for properly forming names of environment
variables vary from one system to another.

<<getenv>> requires a global pointer <<environ>>.
*/

/* This file may have been modified by DJ Delorie (Jan 1991).  If so,
** these modifications are Coyright (C) 1991 DJ Delorie, 24 Kirsten Ave,
** Rochester NH, 03867-2954, USA.
*/

/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the University of California, Berkeley and its contributors''
 * in the documentation or other materials provided with the distribution
 * and in all advertising materials mentioning features or use of this
 * software. Neither the name of the University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

extern char** environ;

/*
 * _findenv --
 *	Returns pointer to value associated with name, if any, else NULL.
 *	Sets offset to be the offset of the name/value combination in the
 *	environmental array, for use by setenv(3) and unsetenv(3).
 *	Explicitly removes '=' in argument name.
 *
 *	This routine *should* be a static; don't use it.
 */

<<<<<<< HEAD
char* _DEFUN(_findenv, (name, offset), register _CONST char* name _AND int* offset)
{
    register int    len;
    register char** p;
    _CONST char*    c;

    c   = name;
    len = 0;
    while (*c && *c != '=') {
        c++;
        len++;
    }

    for (p = environ; *p; ++p)
        if (!strncmp(*p, name, len))
            if (*(c = *p + len) == '=') {
                *offset = p - environ;
                return (char*)(++c);
            }
    return NULL;
=======
char *_DEFUN(_findenv, (name, offset),
             register _CONST char *name _AND int *offset) {
  register int len;
  register char **p;
  _CONST char *c;

  c = name;
  len = 0;
  while (*c && *c != '=') {
    c++;
    len++;
  }

  for (p = environ; *p; ++p)
    if (!strncmp(*p, name, len))
      if (*(c = *p + len) == '=') {
        *offset = p - environ;
        return (char *)(++c);
      }
  return NULL;
>>>>>>> master
}

/*
 * getenv --
 *	Returns ptr to value associated with name, if any, else NULL.
 */

<<<<<<< HEAD
char* _DEFUN(getenv, (name), _CONST char* name)
{
    int   offset;
    char* _findenv();

    return _findenv(name, &offset);
=======
char *_DEFUN(getenv, (name), _CONST char *name) {
  int offset;
  char *_findenv();

  return _findenv(name, &offset);
>>>>>>> master
}
