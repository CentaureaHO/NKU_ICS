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

#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Expand the ungetc buffer `in place'.  That is, adjust fp->_p when
 * the buffer moves, so that it points the same distance from the end,
 * and move the bytes in the buffer around as necessary so that they
 * are all at the end (stack-style).
 */

/*static*/
<<<<<<< HEAD
int __submore(fp) register FILE* fp;
=======
int __submore(fp) register FILE *fp;
>>>>>>> master
{
    register int            i;
    register unsigned char* p;

<<<<<<< HEAD
    if (fp->_ub._base == fp->_ubuf) {
        /*
         * Get a new buffer (rather than expanding the old one).
         */
        if ((p = (unsigned char*)_malloc_r(fp->_data, (size_t)BUFSIZ)) == NULL) return EOF;
        fp->_ub._base = p;
        fp->_ub._size = BUFSIZ;
        p += BUFSIZ - sizeof(fp->_ubuf);
        for (i = sizeof(fp->_ubuf); --i >= 0;) p[i] = fp->_ubuf[i];
        fp->_p                                      = p;
        return 0;
    }
    i = fp->_ub._size;
    p = (unsigned char*)_realloc_r(fp->_data, (_PTR)(fp->_ub._base), i << 1);
    if (p == NULL) return EOF;
    (void)memcpy((void*)(p + i), (void*)p, (size_t)i);
    fp->_p        = p + i;
    fp->_ub._base = p;
    fp->_ub._size = i << 1;
    return 0;
}

int            ungetc(c, fp) int c;
register FILE* fp;
=======
  if (fp->_ub._base == fp->_ubuf) {
    /*
     * Get a new buffer (rather than expanding the old one).
     */
    if ((p = (unsigned char *)_malloc_r(fp->_data, (size_t)BUFSIZ)) == NULL)
      return EOF;
    fp->_ub._base = p;
    fp->_ub._size = BUFSIZ;
    p += BUFSIZ - sizeof(fp->_ubuf);
    for (i = sizeof(fp->_ubuf); --i >= 0;)
      p[i] = fp->_ubuf[i];
    fp->_p = p;
    return 0;
  }
  i = fp->_ub._size;
  p = (unsigned char *)_realloc_r(fp->_data, (_PTR)(fp->_ub._base), i << 1);
  if (p == NULL)
    return EOF;
  (void)memcpy((void *)(p + i), (void *)p, (size_t)i);
  fp->_p = p + i;
  fp->_ub._base = p;
  fp->_ub._size = i << 1;
  return 0;
}

int ungetc(c, fp) int c;
register FILE *fp;
>>>>>>> master
{
    if (c == EOF) return (EOF);

    /* Ensure stdio has been initialized.
       ??? Might be able to remove this as some other stdio routine should
       have already been called to get the char we are un-getting.  */

<<<<<<< HEAD
    CHECK_INIT(fp);
=======
  CHECK_INIT(fp);
>>>>>>> master

    /* After ungetc, we won't be at eof anymore */
    fp->_flags &= ~__SEOF;

<<<<<<< HEAD
    if ((fp->_flags & __SRD) == 0) {
        /*
         * Not already reading: no good unless reading-and-writing.
         * Otherwise, flush any current write stuff.
         */
        if ((fp->_flags & __SRW) == 0) return EOF;
        if (fp->_flags & __SWR) {
            if (fflush(fp)) return EOF;
            fp->_flags &= ~__SWR;
            fp->_w       = 0;
            fp->_lbfsize = 0;
        }
        fp->_flags |= __SRD;
    }
    c = (unsigned char)c;
=======
  if ((fp->_flags & __SRD) == 0) {
    /*
     * Not already reading: no good unless reading-and-writing.
     * Otherwise, flush any current write stuff.
     */
    if ((fp->_flags & __SRW) == 0)
      return EOF;
    if (fp->_flags & __SWR) {
      if (fflush(fp))
        return EOF;
      fp->_flags &= ~__SWR;
      fp->_w = 0;
      fp->_lbfsize = 0;
    }
    fp->_flags |= __SRD;
  }
  c = (unsigned char)c;
>>>>>>> master

    /*
     * If we are in the middle of ungetc'ing, just continue.
     * This may require expanding the current ungetc buffer.
     */

<<<<<<< HEAD
    if (HASUB(fp)) {
        if (fp->_r >= fp->_ub._size && __submore(fp)) return EOF;
        *--fp->_p = c;
        fp->_r++;
        return c;
    }
=======
  if (HASUB(fp)) {
    if (fp->_r >= fp->_ub._size && __submore(fp))
      return EOF;
    *--fp->_p = c;
    fp->_r++;
    return c;
  }
>>>>>>> master

    /*
     * If we can handle this by simply backing up, do so,
     * but never replace the original character.
     * (This makes sscanf() work when scanning `const' data.)
     */

<<<<<<< HEAD
    if (fp->_bf._base != NULL && fp->_p > fp->_bf._base && fp->_p[-1] == c) {
        fp->_p--;
        fp->_r++;
        return c;
    }
=======
  if (fp->_bf._base != NULL && fp->_p > fp->_bf._base && fp->_p[-1] == c) {
    fp->_p--;
    fp->_r++;
    return c;
  }
>>>>>>> master

    /*
     * Create an ungetc buffer.
     * Initially, we will use the `reserve' buffer.
     */

<<<<<<< HEAD
    fp->_ur                          = fp->_r;
    fp->_up                          = fp->_p;
    fp->_ub._base                    = fp->_ubuf;
    fp->_ub._size                    = sizeof(fp->_ubuf);
    fp->_ubuf[sizeof(fp->_ubuf) - 1] = c;
    fp->_p                           = &fp->_ubuf[sizeof(fp->_ubuf) - 1];
    fp->_r                           = 1;
    return c;
=======
  fp->_ur = fp->_r;
  fp->_up = fp->_p;
  fp->_ub._base = fp->_ubuf;
  fp->_ub._size = sizeof(fp->_ubuf);
  fp->_ubuf[sizeof(fp->_ubuf) - 1] = c;
  fp->_p = &fp->_ubuf[sizeof(fp->_ubuf) - 1];
  fp->_r = 1;
  return c;
>>>>>>> master
}
