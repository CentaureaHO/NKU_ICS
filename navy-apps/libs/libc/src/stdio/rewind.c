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
<<rewind>>---reinitialize a file or stream

INDEX
        rewind

ANSI_SYNOPSIS
        #include <stdio.h>
        void rewind(FILE *<[fp]>);

TRAD_SYNOPSIS
        #include <stdio.h>
        void rewind(<[fp]>)
        FILE *<[fp]>;

DESCRIPTION
<<rewind>> returns the file position indicator (if any) for the file
or stream identified by <[fp]> to the beginning of the file.  It also
clears any error indicator and flushes any pending output.

RETURNS
<<rewind>> does not return a result.

PORTABILITY
ANSI C requires <<rewind>>.

No supporting OS subroutines are required.
*/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

#include <stdio.h>

<<<<<<< HEAD
void _DEFUN(rewind, (fp), register FILE* fp)
{
    (void)fflush(fp);
    clearerr(fp);
    if (fp->_seek == NULL) return; /* ??? */
    fp->_r = 0;
    fp->_p = fp->_bf._base;
    (void)(*fp->_seek)(fp->_cookie, (fpos_t)0, SEEK_SET);
=======
void _DEFUN(rewind, (fp), register FILE *fp) {
  (void)fflush(fp);
  clearerr(fp);
  if (fp->_seek == NULL)
    return; /* ??? */
  fp->_r = 0;
  fp->_p = fp->_bf._base;
  (void)(*fp->_seek)(fp->_cookie, (fpos_t)0, SEEK_SET);
>>>>>>> master
}
