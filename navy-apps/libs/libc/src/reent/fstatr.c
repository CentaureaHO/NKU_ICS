/* Reentrant versions of fstat system call.  This implementation just
   calls the fstat system call.  */

#include <_syslist.h>
#include <reent.h>
#include <unistd.h>

/* Some targets provides their own versions of these functions.  Those
   targets should define REENTRANT_SYSCALLS_PROVIDED in TARGET_CFLAGS.  */

#ifdef _REENT_ONLY
#ifndef REENTRANT_SYSCALLS_PROVIDED
#define REENTRANT_SYSCALLS_PROVIDED
#endif
#endif

#ifdef REENTRANT_SYSCALLS_PROVIDED

int _dummy_fstat_syscalls = 1;

#else

/* We use the errno variable used by the system dependent layer.  */
#undef errno
int errno;

/*
FUNCTION
        <<_fstat_r>>---Reentrant version of fstat

INDEX
        _fstat_r

ANSI_SYNOPSIS
        #include <reent.h>
        int _fstat_r(struct _reent *<[ptr]>,
                     int <[fd]>, struct stat *<[pstat]>);

TRAD_SYNOPSIS
        #include <reent.h>
        int _fstat_r(<[ptr]>, <[fd]>, <[pstat]>)
        struct _reent *<[ptr]>;
        int <[fd]>;
        struct stat *<[pstat]>;

DESCRIPTION
        This is a reentrant version of <<fstat>>.  It
        takes a pointer to the global data block, which holds
        <<errno>>.
*/

<<<<<<< HEAD
int          _fstat_r(ptr, fd, pstat) struct _reent* ptr;
int          fd;
struct stat* pstat;
=======
int _fstat_r(ptr, fd, pstat) struct _reent *ptr;
int fd;
struct stat *pstat;
>>>>>>> master
{
    int ret;

<<<<<<< HEAD
    errno                       = 0;
    ret                         = _fstat(fd, pstat);
    if (errno != 0) ptr->_errno = errno;
    return ret;
=======
  errno = 0;
  ret = _fstat(fd, pstat);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
>>>>>>> master
}

#endif /* ! defined (REENTRANT_SYSCALLS_PROVIDED) */
