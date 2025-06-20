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
<<puts>>---write a character string

INDEX
        puts
INDEX
        _puts_r

ANSI_SYNOPSIS
        #include <stdio.h>
        int puts(const char *<[s]>);

        int _puts_r(void *<[reent]>, const char *<[s]>);

TRAD_SYNOPSIS
        #include <stdio.h>
        int puts(<[s]>)
        char *<[s]>;

        int _puts_r(<[reent]>, <[s]>)
        char *<[reent]>;
        char *<[s]>;

DESCRIPTION
<<puts>> writes the string at <[s]> (followed by a newline, instead of
the trailing null) to the standard output stream.

The alternate function <<_puts_r>> is a reentrant version.  The extra
argument <[reent]> is a pointer to a reentrancy structure.

RETURNS
If successful, the result is <<0>>; otherwise, the result is <<EOF>>.

PORTABILITY
ANSI C requires <<puts>>, but does not specify that the result on
success must be <<0>>; any non-negative value is permitted.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

#include "fvwrite.h"
#include <stdio.h>
#include <string.h>

/*
 * Write the given string to stdout, appending a newline.
 */

<<<<<<< HEAD
int _DEFUN(_puts_r, (ptr, s), struct _reent* ptr _AND _CONST char* s)
{
    size_t        c = strlen(s);
    struct __suio uio;
    struct __siov iov[2];
=======
int _DEFUN(_puts_r, (ptr, s), struct _reent *ptr _AND _CONST char *s) {
  size_t c = strlen(s);
  struct __suio uio;
  struct __siov iov[2];
>>>>>>> master

    iov[0].iov_base = s;
    iov[0].iov_len  = c;
    iov[1].iov_base = "\n";
    iov[1].iov_len  = 1;
    uio.uio_resid   = c + 1;
    uio.uio_iov     = &iov[0];
    uio.uio_iovcnt  = 2;

<<<<<<< HEAD
    return (__sfvwrite(_stdout_r(ptr), &uio) ? EOF : '\n');
=======
  return (__sfvwrite(_stdout_r(ptr), &uio) ? EOF : '\n');
>>>>>>> master
}

#ifndef _REENT_ONLY

<<<<<<< HEAD
int _DEFUN(puts, (s), char _CONST* s) { return _puts_r(_REENT, s); }
=======
int _DEFUN(puts, (s), char _CONST *s) { return _puts_r(_REENT, s); }
>>>>>>> master

#endif
