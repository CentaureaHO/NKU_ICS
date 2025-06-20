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

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

#include "fvwrite.h"
#include "local.h"
#include <stdio.h>

/*
 * Write the given character into the (probably full) buffer for
 * the given file.  Flush the buffer out if it is or becomes full,
 * or if c=='\n' and the file is line buffered.
 */

<<<<<<< HEAD
int            __swbuf(c, fp) register int c;
register FILE* fp;
=======
int __swbuf(c, fp) register int c;
register FILE *fp;
>>>>>>> master
{
    register int n;

    /* Ensure stdio has been initialized.  */

<<<<<<< HEAD
    CHECK_INIT(fp);
=======
  CHECK_INIT(fp);
>>>>>>> master

    /*
     * In case we cannot write, or longjmp takes us out early,
     * make sure _w is 0 (if fully- or un-buffered) or -_bf._size
     * (if line buffered) so that we will get called again.
     * If we did not do this, a sufficient number of putc()
     * calls might wrap _w from negative to positive.
     */

<<<<<<< HEAD
    fp->_w = fp->_lbfsize;
    if (cantwrite(fp)) return EOF;
    c = (unsigned char)c;
=======
  fp->_w = fp->_lbfsize;
  if (cantwrite(fp))
    return EOF;
  c = (unsigned char)c;
>>>>>>> master

    /*
     * If it is completely full, flush it out.  Then, in any case,
     * stuff c into the buffer.  If this causes the buffer to fill
     * completely, or if c is '\n' and the file is line buffered,
     * flush it (perhaps a second time).  The second flush will always
     * happen on unbuffered streams, where _bf._size==1; fflush()
     * guarantees that putc() will always call wbuf() by setting _w
     * to 0, so we need not do anything else.
     */

<<<<<<< HEAD
    n = fp->_p - fp->_bf._base;
    if (n >= fp->_bf._size) {
        if (fflush(fp)) return EOF;
        n = 0;
    }
    fp->_w--;
    *fp->_p++ = c;
    if (++n == fp->_bf._size || (fp->_flags & __SLBF && c == '\n'))
        if (fflush(fp)) return EOF;
    return c;
=======
  n = fp->_p - fp->_bf._base;
  if (n >= fp->_bf._size) {
    if (fflush(fp))
      return EOF;
    n = 0;
  }
  fp->_w--;
  *fp->_p++ = c;
  if (++n == fp->_bf._size || (fp->_flags & __SLBF && c == '\n'))
    if (fflush(fp))
      return EOF;
  return c;
>>>>>>> master
}
