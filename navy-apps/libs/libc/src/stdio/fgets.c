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

/*

FUNCTION
        <<fgets>>---get character string from a file or stream
INDEX
        fgets

ANSI_SYNOPSIS
        #include <stdio.h>
        char *fgets(char *<[buf]>, int <[n]>, FILE *<[fp]>);

TRAD_SYNOPSIS
        #include <stdio.h>
        char *fgets(<[buf]>,<[n]>,<[fp]>)
        char *<[buf]>;
        int <[n]>;
        FILE *<[fp]>;

DESCRIPTION
        Reads at most <[n-1]> characters from <[fp]> until a newline
        is found. The characters including to the newline are stored
        in <[buf]>. The buffer is terminated with a 0.


RETURNS
        <<fgets>> returns the buffer passed to it, with the data
        filled in. If end of file occurs with some data already
        accumulated, the data is returned with no other indication. If
        no data are read, NULL is returned instead.

PORTABILITY
        <<fgets>> should replace all uses of <<gets>>. Note however
        that <<fgets>> returns all of the data, while <<gets>> removes
        the trailing newline (with no indication that it has done so.)

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

#include <stdio.h>
#include <string.h>

extern int __srefill();

/*
 * Read at most n-1 characters from the given file.
 * Stop when a newline has been read, or the count runs out.
 * Return first argument, or NULL if no characters were read.
 */

<<<<<<< HEAD
char* _DEFUN(fgets, (buf, n, fp), char* buf _AND int n _AND FILE* fp)
{
    size_t         len;
    char*          s;
    unsigned char *p, *t;

    if (n < 2) /* sanity check */
        return 0;

    s = buf;
    n--; /* leave space for NUL */
    do
    {
        /*
         * If the buffer is empty, refill it.
         */
        if ((len = fp->_r) <= 0) {
            if (__srefill(fp)) {
                /* EOF: stop with partial or no line */
                if (s == buf) return 0;
                break;
            }
            len = fp->_r;
        }
        p = fp->_p;

        /*
         * Scan through at most n bytes of the current buffer,
         * looking for '\n'.  If found, copy up to and including
         * newline, and stop.  Otherwise, copy entire chunk
         * and loop.
         */
        if (len > n) len = n;
        t                = (unsigned char*)memchr((_PTR)p, '\n', len);
        if (t != 0) {
            len = ++t - p;
            fp->_r -= len;
            fp->_p = t;
            (void)memcpy((_PTR)s, (_PTR)p, len);
            s[len] = 0;
            return (buf);
        }
        fp->_r -= len;
        fp->_p += len;
        (void)memcpy((_PTR)s, (_PTR)p, len);
        s += len;
    } while ((n -= len) != 0);
    *s = 0;
    return buf;
=======
char *_DEFUN(fgets, (buf, n, fp), char *buf _AND int n _AND FILE *fp) {
  size_t len;
  char *s;
  unsigned char *p, *t;

  if (n < 2) /* sanity check */
    return 0;

  s = buf;
  n--; /* leave space for NUL */
  do {
    /*
     * If the buffer is empty, refill it.
     */
    if ((len = fp->_r) <= 0) {
      if (__srefill(fp)) {
        /* EOF: stop with partial or no line */
        if (s == buf)
          return 0;
        break;
      }
      len = fp->_r;
    }
    p = fp->_p;

    /*
     * Scan through at most n bytes of the current buffer,
     * looking for '\n'.  If found, copy up to and including
     * newline, and stop.  Otherwise, copy entire chunk
     * and loop.
     */
    if (len > n)
      len = n;
    t = (unsigned char *)memchr((_PTR)p, '\n', len);
    if (t != 0) {
      len = ++t - p;
      fp->_r -= len;
      fp->_p = t;
      (void)memcpy((_PTR)s, (_PTR)p, len);
      s[len] = 0;
      return (buf);
    }
    fp->_r -= len;
    fp->_p += len;
    (void)memcpy((_PTR)s, (_PTR)p, len);
    s += len;
  } while ((n -= len) != 0);
  *s = 0;
  return buf;
>>>>>>> master
}
