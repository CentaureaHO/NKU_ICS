/* Reentrant versions of stat system call.  This implementation just
   calls the stat system call.  */

#include <_syslist.h>
#include <reent.h>
#include <unistd.h>

/* Some targets provides their own versions of these functions.  Those
   targets should define REENTRANT_SYSCALLS_PROVIDED in
   TARGET_CFLAGS.  */

#ifdef _REENT_ONLY
#ifndef REENTRANT_SYSCALLS_PROVIDED
#define REENTRANT_SYSCALLS_PROVIDED
#endif
#endif

#ifdef REENTRANT_SYSCALLS_PROVIDED

int _dummy_stat_syscalls = 1;

#else

/* We use the errno variable used by the system dependent layer.  */
#undef errno
int errno;

/*
FUNCTION
        <<_stat_r>>---Reentrant version of stat

INDEX
        _stat_r

ANSI_SYNOPSIS
        #include <reent.h>
        int _stat_r(struct _reent *<[ptr]>,
                    const char *<[file]>, struct stat *<[pstat]>);

TRAD_SYNOPSIS
        #include <reent.h>
        int _stat_r(<[ptr]>, <[file]>, <[pstat]>)
        struct _reent *<[ptr]>;
        char *<[file]>;
        struct stat *<[pstat]>;

DESCRIPTION
        This is a reentrant version of <<stat>>.  It
        takes a pointer to the global data block, which holds
        <<errno>>.
*/

<<<<<<< HEAD
int          _stat_r(ptr, file, pstat) struct _reent* ptr;
_CONST char* file;
struct stat* pstat;
=======
int _stat_r(ptr, file, pstat) struct _reent *ptr;
_CONST char *file;
struct stat *pstat;
>>>>>>> master
{
    int ret;

<<<<<<< HEAD
    errno                       = 0;
    ret                         = _stat(file, pstat);
    if (errno != 0) ptr->_errno = errno;
    return ret;
=======
  errno = 0;
  ret = _stat(file, pstat);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
>>>>>>> master
}

#endif /* ! defined (REENTRANT_SYSCALLS_PROVIDED) */
