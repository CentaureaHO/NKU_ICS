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
<<fseek>>---set file position

INDEX
        fseek

ANSI_SYNOPSIS
        #include <stdio.h>
        int fseek(FILE *<[fp]>, long <[offset]>, int <[whence]>)

TRAD_SYNOPSIS
        #include <stdio.h>
        int fseek(<[fp]>, <[offset]>, <[whence]>)
        FILE *<[fp]>;
        long <[offset]>;
        int <[whence]>;

DESCRIPTION
Objects of type <<FILE>> can have a ``position'' that records how much
of the file your program has already read.  Many of the <<stdio>> functions
depend on this position, and many change it as a side effect.

You can use <<fseek>> to set the position for the file identified by
<[fp]>.  The value of <[offset]> determines the new position, in one
of three ways selected by the value of <[whence]> (defined as macros
in `<<stdio.h>>'):

<<SEEK_SET>>---<[offset]> is the absolute file position (an offset
from the beginning of the file) desired.  <[offset]> must be positive.

<<SEEK_CUR>>---<[offset]> is relative to the current file position.
<[offset]> can meaningfully be either positive or negative.

<<SEEK_END>>---<[offset]> is relative to the current end of file.
<[offset]> can meaningfully be either positive (to increase the size
of the file) or negative.

See <<ftell>> to determine the current file position.

RETURNS
<<fseek>> returns <<0>> when successful.  If <<fseek>> fails, the
result is <<EOF>>.  The reason for failure is indicated in <<errno>>:
either <<ESPIPE>> (the stream identified by <[fp]> doesn't support
repositioning) or <<EINVAL>> (invalid file position).

PORTABILITY
ANSI C requires <<fseek>>.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

#include "local.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#define POS_ERR (-(fpos_t)1)

/*
 * Seek the given file to the given offset.
 * `Whence' must be one of the three SEEK_* macros.
 */

<<<<<<< HEAD
int  fseek(fp, offset, whence) register FILE* fp;
long offset;
int  whence;
{
    struct _reent* ptr;
    fpos_t         _EXFUN((*seekfn), (void*, fpos_t, int));
    fpos_t         target, curoff;
    size_t         n;
    struct stat    st;
    int            havepos;
=======
int fseek(fp, offset, whence) register FILE *fp;
long offset;
int whence;
{
  struct _reent *ptr;
  fpos_t _EXFUN((*seekfn), (void *, fpos_t, int));
  fpos_t target, curoff;
  size_t n;
  struct stat st;
  int havepos;
>>>>>>> master

    /* Make sure stdio is set up.  */

<<<<<<< HEAD
    CHECK_INIT(fp);
    ptr = fp->_data;
=======
  CHECK_INIT(fp);
  ptr = fp->_data;
>>>>>>> master

    /* If we've been doing some writing, and we're in append mode
       then we don't really know where the filepos is.  */

<<<<<<< HEAD
    if (fp->_flags & __SAPP && fp->_flags & __SWR) {
        /* So flush the buffer and seek to the end.  */
        fflush(fp);
    }
=======
  if (fp->_flags & __SAPP && fp->_flags & __SWR) {
    /* So flush the buffer and seek to the end.  */
    fflush(fp);
  }
>>>>>>> master

    /* Have to be able to seek.  */

<<<<<<< HEAD
    if ((seekfn = fp->_seek) == NULL) {
        ptr->_errno = ESPIPE; /* ??? */
        return EOF;
    }
=======
  if ((seekfn = fp->_seek) == NULL) {
    ptr->_errno = ESPIPE; /* ??? */
    return EOF;
  }
>>>>>>> master

    /*
     * Change any SEEK_CUR to SEEK_SET, and check `whence' argument.
     * After this, whence is either SEEK_SET or SEEK_END.
     */

<<<<<<< HEAD
    switch (whence)
    {
        case SEEK_CUR:
            /*
             * In order to seek relative to the current stream offset,
             * we have to first find the current stream offset a la
             * ftell (see ftell for details).
             */
            if (fp->_flags & __SOFF)
                curoff = fp->_offset;
            else
            {
                curoff = (*seekfn)(fp->_cookie, (fpos_t)0, SEEK_CUR);
                if (curoff == -1L) return EOF;
            }
            if (fp->_flags & __SRD) {
                curoff -= fp->_r;
                if (HASUB(fp)) curoff -= fp->_ur;
            }
            else if (fp->_flags & __SWR && fp->_p != NULL)
                curoff += fp->_p - fp->_bf._base;

            offset += curoff;
            whence  = SEEK_SET;
            havepos = 1;
            break;

        case SEEK_SET:
        case SEEK_END: havepos = 0; break;

        default: ptr->_errno = EINVAL; return (EOF);
=======
  switch (whence) {
  case SEEK_CUR:
    /*
     * In order to seek relative to the current stream offset,
     * we have to first find the current stream offset a la
     * ftell (see ftell for details).
     */
    if (fp->_flags & __SOFF)
      curoff = fp->_offset;
    else {
      curoff = (*seekfn)(fp->_cookie, (fpos_t)0, SEEK_CUR);
      if (curoff == -1L)
        return EOF;
>>>>>>> master
    }
    if (fp->_flags & __SRD) {
      curoff -= fp->_r;
      if (HASUB(fp))
        curoff -= fp->_ur;
    } else if (fp->_flags & __SWR && fp->_p != NULL)
      curoff += fp->_p - fp->_bf._base;

    offset += curoff;
    whence = SEEK_SET;
    havepos = 1;
    break;

  case SEEK_SET:
  case SEEK_END:
    havepos = 0;
    break;

  default:
    ptr->_errno = EINVAL;
    return (EOF);
  }

    /*
     * Can only optimise if:
     *	reading (and not reading-and-writing);
     *	not unbuffered; and
     *	this is a `regular' Unix file (and hence seekfn==__sseek).
     * We must check __NBF first, because it is possible to have __NBF
     * and __SOPT both set.
     */

<<<<<<< HEAD
    if (fp->_bf._base == NULL) __smakebuf(fp);
    if (fp->_flags & (__SWR | __SRW | __SNBF | __SNPT)) goto dumb;
    if ((fp->_flags & __SOPT) == 0) {
        if (seekfn != __sseek || fp->_file < 0 || _fstat_r(ptr, fp->_file, &st) || (st.st_mode & S_IFMT) != S_IFREG) {
            fp->_flags |= __SNPT;
            goto dumb;
        }
#ifdef HAVE_BLKSIZE
        fp->_blksize = st.st_blksize;
#else
        fp->_blksize = 1024;
#endif
        fp->_flags |= __SOPT;
=======
  if (fp->_bf._base == NULL)
    __smakebuf(fp);
  if (fp->_flags & (__SWR | __SRW | __SNBF | __SNPT))
    goto dumb;
  if ((fp->_flags & __SOPT) == 0) {
    if (seekfn != __sseek || fp->_file < 0 || _fstat_r(ptr, fp->_file, &st) ||
        (st.st_mode & S_IFMT) != S_IFREG) {
      fp->_flags |= __SNPT;
      goto dumb;
>>>>>>> master
    }
#ifdef HAVE_BLKSIZE
    fp->_blksize = st.st_blksize;
#else
    fp->_blksize = 1024;
#endif
    fp->_flags |= __SOPT;
  }

    /*
     * We are reading; we can try to optimise.
     * Figure out where we are going and where we are now.
     */

<<<<<<< HEAD
    if (whence == SEEK_SET)
        target = offset;
    else
    {
        if (_fstat_r(ptr, fp->_file, &st)) goto dumb;
        target = st.st_size + offset;
    }

    if (!havepos) {
        if (fp->_flags & __SOFF)
            curoff = fp->_offset;
        else
        {
            curoff = (*seekfn)(fp->_cookie, 0L, SEEK_CUR);
            if (curoff == POS_ERR) goto dumb;
        }
        curoff -= fp->_r;
        if (HASUB(fp)) curoff -= fp->_ur;
    }

    /*
     * Compute the number of bytes in the input buffer (pretending
     * that any ungetc() input has been discarded).  Adjust current
     * offset backwards by this count so that it represents the
     * file offset for the first byte in the current input buffer.
     */

    if (HASUB(fp)) {
        n = fp->_up - fp->_bf._base;
        curoff -= n;
        n += fp->_ur;
    }
    else
    {
        n = fp->_p - fp->_bf._base;
        curoff -= n;
        n += fp->_r;
=======
  if (whence == SEEK_SET)
    target = offset;
  else {
    if (_fstat_r(ptr, fp->_file, &st))
      goto dumb;
    target = st.st_size + offset;
  }

  if (!havepos) {
    if (fp->_flags & __SOFF)
      curoff = fp->_offset;
    else {
      curoff = (*seekfn)(fp->_cookie, 0L, SEEK_CUR);
      if (curoff == POS_ERR)
        goto dumb;
>>>>>>> master
    }
    curoff -= fp->_r;
    if (HASUB(fp))
      curoff -= fp->_ur;
  }

    /*
     * If the target offset is within the current buffer,
     * simply adjust the pointers, clear EOF, undo ungetc(),
     * and return.  (If the buffer was modified, we have to
     * skip this; see fgetline.c.)
     */

<<<<<<< HEAD
    if ((fp->_flags & __SMOD) == 0 && target >= curoff && target < curoff + n) {
        register int o = target - curoff;

        fp->_p = fp->_bf._base + o;
        fp->_r = n - o;
        if (HASUB(fp)) FREEUB(fp);
        fp->_flags &= ~__SEOF;
        return 0;
    }
=======
  if (HASUB(fp)) {
    n = fp->_up - fp->_bf._base;
    curoff -= n;
    n += fp->_ur;
  } else {
    n = fp->_p - fp->_bf._base;
    curoff -= n;
    n += fp->_r;
  }
>>>>>>> master

    /*
     * The place we want to get to is not within the current buffer,
     * but we can still be kind to the kernel copyout mechanism.
     * By aligning the file offset to a block boundary, we can let
     * the kernel use the VM hardware to map pages instead of
     * copying bytes laboriously.  Using a block boundary also
     * ensures that we only read one block, rather than two.
     */

<<<<<<< HEAD
    curoff = target & ~(fp->_blksize - 1);
    if ((*seekfn)(fp->_cookie, curoff, SEEK_SET) == POS_ERR) goto dumb;
    fp->_r = 0;
    if (HASUB(fp)) FREEUB(fp);
    fp->_flags &= ~__SEOF;
    n = target - curoff;
    if (n) {
        if (__srefill(fp) || fp->_r < n) goto dumb;
        fp->_p += n;
        fp->_r -= n;
    }
    return 0;

=======
  if ((fp->_flags & __SMOD) == 0 && target >= curoff && target < curoff + n) {
    register int o = target - curoff;

    fp->_p = fp->_bf._base + o;
    fp->_r = n - o;
    if (HASUB(fp))
      FREEUB(fp);
    fp->_flags &= ~__SEOF;
    return 0;
  }

  /*
   * The place we want to get to is not within the current buffer,
   * but we can still be kind to the kernel copyout mechanism.
   * By aligning the file offset to a block boundary, we can let
   * the kernel use the VM hardware to map pages instead of
   * copying bytes laboriously.  Using a block boundary also
   * ensures that we only read one block, rather than two.
   */

  curoff = target & ~(fp->_blksize - 1);
  if ((*seekfn)(fp->_cookie, curoff, SEEK_SET) == POS_ERR)
    goto dumb;
  fp->_r = 0;
  if (HASUB(fp))
    FREEUB(fp);
  fp->_flags &= ~__SEOF;
  n = target - curoff;
  if (n) {
    if (__srefill(fp) || fp->_r < n)
      goto dumb;
    fp->_p += n;
    fp->_r -= n;
  }
  return 0;

>>>>>>> master
/*
 * We get here if we cannot optimise the seek ... just
 * do it.  Allow the seek function to change fp->_bf._base.
 */

dumb:
<<<<<<< HEAD
    if (fflush(fp) || (*seekfn)(fp->_cookie, offset, whence) == POS_ERR) return EOF;
    /* success: clear EOF indicator and discard ungetc() data */
    if (HASUB(fp)) FREEUB(fp);
    fp->_p = fp->_bf._base;
    fp->_r = 0;
    /* fp->_w = 0; */ /* unnecessary (I think...) */
    fp->_flags &= ~__SEOF;
    return 0;
=======
  if (fflush(fp) || (*seekfn)(fp->_cookie, offset, whence) == POS_ERR)
    return EOF;
  /* success: clear EOF indicator and discard ungetc() data */
  if (HASUB(fp))
    FREEUB(fp);
  fp->_p = fp->_bf._base;
  fp->_r = 0;
  /* fp->_w = 0; */ /* unnecessary (I think...) */
  fp->_flags &= ~__SEOF;
  return 0;
>>>>>>> master
}
