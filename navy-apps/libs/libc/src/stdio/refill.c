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
#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
static int lflush(fp) FILE* fp;
{
    if ((fp->_flags & (__SLBF | __SWR)) == __SLBF | __SWR) return fflush(fp);
    return 0;
=======
static int lflush(fp) FILE *fp;
{
  if ((fp->_flags & (__SLBF | __SWR)) == __SLBF | __SWR)
    return fflush(fp);
  return 0;
>>>>>>> master
}

/*
 * Refill a stdio buffer.
 * Return EOF on eof or error, 0 otherwise.
 */

<<<<<<< HEAD
int _DEFUN(__srefill, (fp), register FILE* fp)
{
    /* make sure stdio is set up */

    CHECK_INIT(fp);

    fp->_r = 0; /* largely a convenience for callers */
=======
int _DEFUN(__srefill, (fp), register FILE *fp) {
  /* make sure stdio is set up */

  CHECK_INIT(fp);

  fp->_r = 0; /* largely a convenience for callers */
>>>>>>> master

    /* SysV does not make this test; take it out for compatibility */
    if (fp->_flags & __SEOF) return EOF;

<<<<<<< HEAD
    /* if not already reading, have to be reading and writing */
    if ((fp->_flags & __SRD) == 0) {
        if ((fp->_flags & __SRW) == 0) return EOF;
        /* switch to reading */
        if (fp->_flags & __SWR) {
            if (fflush(fp)) return EOF;
            fp->_flags &= ~__SWR;
            fp->_w       = 0;
            fp->_lbfsize = 0;
        }
        fp->_flags |= __SRD;
    }
    else
    {
        /*
         * We were reading.  If there is an ungetc buffer,
         * we must have been reading from that.  Drop it,
         * restoring the previous buffer (if any).  If there
         * is anything in that buffer, return.
         */
        if (HASUB(fp)) {
            FREEUB(fp);
            if ((fp->_r = fp->_ur) != 0) {
                fp->_p = fp->_up;
                return 0;
            }
        }
=======
  /* if not already reading, have to be reading and writing */
  if ((fp->_flags & __SRD) == 0) {
    if ((fp->_flags & __SRW) == 0)
      return EOF;
    /* switch to reading */
    if (fp->_flags & __SWR) {
      if (fflush(fp))
        return EOF;
      fp->_flags &= ~__SWR;
      fp->_w = 0;
      fp->_lbfsize = 0;
    }
    fp->_flags |= __SRD;
  } else {
    /*
     * We were reading.  If there is an ungetc buffer,
     * we must have been reading from that.  Drop it,
     * restoring the previous buffer (if any).  If there
     * is anything in that buffer, return.
     */
    if (HASUB(fp)) {
      FREEUB(fp);
      if ((fp->_r = fp->_ur) != 0) {
        fp->_p = fp->_up;
        return 0;
      }
>>>>>>> master
    }
  }

<<<<<<< HEAD
    if (fp->_bf._base == NULL) __smakebuf(fp);
=======
  if (fp->_bf._base == NULL)
    __smakebuf(fp);
>>>>>>> master

    /*
     * Before reading from a line buffered or unbuffered file,
     * flush all line buffered output files, per the ANSI C
     * standard.
     */

<<<<<<< HEAD
    if (fp->_flags & (__SLBF | __SNBF)) (void)_fwalk(fp->_data, lflush);
    fp->_p = fp->_bf._base;
    fp->_r = (*fp->_read)(fp->_cookie, (char*)fp->_p, fp->_bf._size);
    fp->_flags &= ~__SMOD; /* buffer contents are again pristine */
    if (fp->_r <= 0) {
        if (fp->_r == 0)
            fp->_flags |= __SEOF;
        else
        {
            fp->_r = 0;
            fp->_flags |= __SERR;
        }
        return EOF;
    }
    return 0;
=======
  if (fp->_flags & (__SLBF | __SNBF))
    (void)_fwalk(fp->_data, lflush);
  fp->_p = fp->_bf._base;
  fp->_r = (*fp->_read)(fp->_cookie, (char *)fp->_p, fp->_bf._size);
  fp->_flags &= ~__SMOD; /* buffer contents are again pristine */
  if (fp->_r <= 0) {
    if (fp->_r == 0)
      fp->_flags |= __SEOF;
    else {
      fp->_r = 0;
      fp->_flags |= __SERR;
    }
    return EOF;
  }
  return 0;
>>>>>>> master
}
