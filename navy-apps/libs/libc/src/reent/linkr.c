/* Reentrant versions of file system calls.  These implementations
   just call the usual system calls.  */

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

int _dummy_link_syscalls = 1;

#else

/* We use the errno variable used by the system dependent layer.  */
#undef errno
int errno;

/*
FUNCTION
        <<_link_r>>---Reentrant version of link

INDEX
        _link_r

ANSI_SYNOPSIS
        #include <reent.h>
        int _link_r(struct _reent *<[ptr]>,
                    const char *<[old]>, const char *<[new]>);

TRAD_SYNOPSIS
        #include <reent.h>
        int _link_r(<[ptr]>, <[old]>, <[new]>)
        struct _reent *<[ptr]>;
        char *<[old]>;
        char *<[new]>;

DESCRIPTION
        This is a reentrant version of <<link>>.  It
        takes a pointer to the global data block, which holds
        <<errno>>.
*/

<<<<<<< HEAD
int          _link_r(ptr, old, new) struct _reent* ptr;
_CONST char* old;
_CONST char* new;
=======
int _link_r(ptr, old, new) struct _reent *ptr;
_CONST char *old;
_CONST char *new;
>>>>>>> master
{
    int ret;

<<<<<<< HEAD
    errno                       = 0;
    ret                         = _link(old, new);
    if (errno != 0) ptr->_errno = errno;
    return ret;
=======
  errno = 0;
  ret = _link(old, new);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
>>>>>>> master
}

/*
FUNCTION
        <<_unlink_r>>---Reentrant version of unlink

INDEX
        _unlink_r

ANSI_SYNOPSIS
        #include <reent.h>
        int _unlink_r(struct _reent *<[ptr]>, const char *<[file]>);

TRAD_SYNOPSIS
        #include <reent.h>
        int _unlink_r(<[ptr]>, <[file]>)
        struct _reent *<[ptr]>;
        char *<[file]>;

DESCRIPTION
        This is a reentrant version of <<unlink>>.  It
        takes a pointer to the global data block, which holds
        <<errno>>.
*/

<<<<<<< HEAD
int          _unlink_r(ptr, file) struct _reent* ptr;
_CONST char* file;
=======
int _unlink_r(ptr, file) struct _reent *ptr;
_CONST char *file;
>>>>>>> master
{
    int ret;

<<<<<<< HEAD
    errno                       = 0;
    ret                         = _unlink(file);
    if (errno != 0) ptr->_errno = errno;
    return ret;
=======
  errno = 0;
  ret = _unlink(file);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
>>>>>>> master
}

#endif /* ! defined (REENTRANT_SYSCALLS_PROVIDED) */
