/* No user fns here.  Pesch 15apr92. */

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
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>

/*
 * Small standard I/O/seek/close functions.
 * These maintain the `known seek offset' for seek optimisation.
 */

<<<<<<< HEAD
int   __sread(cookie, buf, n) _PTR cookie;
char* buf;
int   n;
{
    register FILE* fp = (FILE*)cookie;
    register int   ret;

    ret = _read_r(fp->_data, fp->_file, buf, n);
=======
int __sread(cookie, buf, n) _PTR cookie;
char *buf;
int n;
{
  register FILE *fp = (FILE *)cookie;
  register int ret;

  ret = _read_r(fp->_data, fp->_file, buf, n);
>>>>>>> master

    /* If the read succeeded, update the current offset.  */

<<<<<<< HEAD
    if (ret >= 0)
        fp->_offset += ret;
    else
        fp->_flags &= ~__SOFF; /* paranoia */
    return ret;
}

int  __swrite(cookie, buf, n) _PTR cookie;
char _CONST* buf;
int          n;
{
    register FILE* fp = (FILE*)cookie;

    if (fp->_flags & __SAPP) (void)_lseek_r(fp->_data, fp->_file, (off_t)0, SEEK_END);
    fp->_flags &= ~__SOFF; /* in case O_APPEND mode is set */
    return _write_r(fp->_data, fp->_file, buf, n);
=======
  if (ret >= 0)
    fp->_offset += ret;
  else
    fp->_flags &= ~__SOFF; /* paranoia */
  return ret;
}

int __swrite(cookie, buf, n) _PTR cookie;
char _CONST *buf;
int n;
{
  register FILE *fp = (FILE *)cookie;

  if (fp->_flags & __SAPP)
    (void)_lseek_r(fp->_data, fp->_file, (off_t)0, SEEK_END);
  fp->_flags &= ~__SOFF; /* in case O_APPEND mode is set */
  return _write_r(fp->_data, fp->_file, buf, n);
>>>>>>> master
}

fpos_t __sseek(cookie, offset, whence) _PTR cookie;
fpos_t offset;
<<<<<<< HEAD
int    whence;
{
    register FILE* fp = (FILE*)cookie;
    register off_t ret;

    ret = _lseek_r(fp->_data, fp->_file, (off_t)offset, whence);
    if (ret == -1L)
        fp->_flags &= ~__SOFF;
    else
    {
        fp->_flags |= __SOFF;
        fp->_offset = ret;
    }
    return ret;
=======
int whence;
{
  register FILE *fp = (FILE *)cookie;
  register off_t ret;

  ret = _lseek_r(fp->_data, fp->_file, (off_t)offset, whence);
  if (ret == -1L)
    fp->_flags &= ~__SOFF;
  else {
    fp->_flags |= __SOFF;
    fp->_offset = ret;
  }
  return ret;
>>>>>>> master
}

int __sclose(cookie) _PTR cookie;
{
<<<<<<< HEAD
    FILE* fp = (FILE*)cookie;

    return _close_r(fp->_data, fp->_file);
=======
  FILE *fp = (FILE *)cookie;

  return _close_r(fp->_data, fp->_file);
>>>>>>> master
}
