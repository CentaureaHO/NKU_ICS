/*
FUNCTION
<<fdopen>>---turn open file into a stream

INDEX
        fdopen
INDEX
        _fdopen_r

ANSI_SYNOPSIS
        #include <stdio.h>
        FILE *fdopen(int <[fd]>, const char *<[mode]>);
        FILE *_fdopen_r(void *<[reent]>,
                     int <[fd]>, const char *<[mode]>);

TRAD_SYNOPSIS
        #include <stdio.h>
        FILE *fdopen(<[fd]>, <[mode]>)
        int <[fd]>;
        char *<[mode]>;

        FILE *_fdopen_r(<[reent]>, <[fd]>, <[mode]>)
        char *<[reent]>;
        int <[fd]>;
        char *<[mode]>);

DESCRIPTION
<<fdopen>> produces a file descriptor of type <<FILE *>>, from a
descriptor for an already-open file (returned, for example, by the
system subroutine <<open>> rather than by <<fopen>>).
The <[mode]> argument has the same meanings as in <<fopen>>.

RETURNS
File pointer or <<NULL>>, as for <<fopen>>.

PORTABILITY
<<fdopen>> is ANSI.
*/

#include <sys/types.h>

#include "local.h"
#include <_syslist.h>
#include <errno.h>
#include <stdio.h>

extern int __sflags();

<<<<<<< HEAD
FILE* _DEFUN(_fdopen_r, (ptr, fd, mode), struct _reent* ptr _AND int fd _AND _CONST char* mode)
{
    register FILE* fp;
    int            flags, oflags;

    if ((flags = __sflags(ptr, mode, &oflags)) == 0) return 0;

/* make sure the mode the user wants is a subset of the actual mode */
#ifdef F_GETFL
    if ((fdflags = _fcntl(fd, F_GETFL, 0)) < 0) return 0;
    fdmode = fdflags & O_ACCMODE;
    if (fdmode != O_RDWR && (fdmode != (oflags & O_ACCMODE))) {
        ptr->_errno = EBADF;
        return 0;
    }
#endif

    if ((fp = __sfp(ptr)) == 0) return 0;
    fp->_flags = flags;
=======
FILE *_DEFUN(_fdopen_r, (ptr, fd, mode),
             struct _reent *ptr _AND int fd _AND _CONST char *mode) {
  register FILE *fp;
  int flags, oflags;

  if ((flags = __sflags(ptr, mode, &oflags)) == 0)
    return 0;

/* make sure the mode the user wants is a subset of the actual mode */
#ifdef F_GETFL
  if ((fdflags = _fcntl(fd, F_GETFL, 0)) < 0)
    return 0;
  fdmode = fdflags & O_ACCMODE;
  if (fdmode != O_RDWR && (fdmode != (oflags & O_ACCMODE))) {
    ptr->_errno = EBADF;
    return 0;
  }
#endif

  if ((fp = __sfp(ptr)) == 0)
    return 0;
  fp->_flags = flags;
>>>>>>> master
/*
 * If opened for appending, but underlying descriptor
 * does not have O_APPEND bit set, assert __SAPP so that
 * __swrite() will lseek to end before each write.
 */
#ifdef F_GETFL
    if ((oflags & O_APPEND) && !(fdflags & O_APPEND))
#endif
<<<<<<< HEAD
        fp->_flags |= __SAPP;
    fp->_file   = fd;
    fp->_cookie = (_PTR)fp;
=======
    fp->_flags |= __SAPP;
  fp->_file = fd;
  fp->_cookie = (_PTR)fp;
>>>>>>> master

#undef _read
#undef _write
#undef _seek
#undef _close

    fp->_read  = __sread;
    fp->_write = __swrite;
    fp->_seek  = __sseek;
    fp->_close = __sclose;
    return fp;
}

#ifndef _REENT_ONLY

<<<<<<< HEAD
FILE* _DEFUN(fdopen, (fd, mode), int fd _AND _CONST char* mode) { return _fdopen_r(_REENT, fd, mode); }
=======
FILE *_DEFUN(fdopen, (fd, mode), int fd _AND _CONST char *mode) {
  return _fdopen_r(_REENT, fd, mode);
}
>>>>>>> master

#endif
