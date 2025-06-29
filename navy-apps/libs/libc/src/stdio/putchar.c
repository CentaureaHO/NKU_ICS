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
<<putchar>>---write a character (macro)

INDEX
        putchar
INDEX
        _putchar_r

ANSI_SYNOPSIS
        #include <stdio.h>
        int putchar(int <[ch]>);

        int _putchar_r(void *<[reent]>, int <[ch]>);

TRAD_SYNOPSIS
        #include <stdio.h>
        int putchar(<[ch]>)
        int <[ch]>;

        int _putchar_r(<[reent]>, <[ch]>)
        char *<[reent]>;
        int <[ch]>;

DESCRIPTION
<<putchar>> is a macro, defined in <<stdio.h>>.  <<putchar>>
writes its argument to the standard output stream,
after converting it from an <<int>> to an <<unsigned char>>.

The alternate function <<_putchar_r>> is a reentrant version.  The
extra argument <[reent]> is a pointer to a reentrancy structure.

RETURNS
If successful, <<putchar>> returns its argument <[ch]>.  If an error
intervenes, the result is <<EOF>>.  You can use `<<ferror(stdin)>>' to
query for errors.

PORTABILITY
ANSI C requires <<putchar>>; it suggests, but does not require, that
<<putchar>> be implemented as a macro.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

/*
 * A subroutine version of the macro putchar
 */

#include <stdio.h>

#undef putchar

<<<<<<< HEAD
int _putchar_r(ptr, c) struct _reent* ptr;
int c;
{
    return __sputc(c, _stdout_r(ptr));
}
=======
int _putchar_r(ptr, c) struct _reent *ptr;
int c;
{ return __sputc(c, _stdout_r(ptr)); }
>>>>>>> master

#ifndef _REENT_ONLY

int putchar(c) int c;
{
    /* CHECK_INIT is (eventually) called by __swbuf.  */

<<<<<<< HEAD
    _putchar_r(_REENT, c);
=======
  _putchar_r(_REENT, c);
>>>>>>> master
}

#endif
