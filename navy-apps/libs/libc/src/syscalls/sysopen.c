/* connector for open */

#include <fcntl.h>
#include <reent.h>

#ifdef _HAVE_STDC

/* The prototype in <fcntl.h> uses ..., so we must correspond.  */

#include <stdarg.h>

<<<<<<< HEAD
int open(const char* file, int flags, ...)
{
    va_list ap;
    int     ret;

    va_start(ap, flags);
#ifdef REENTRANT_SYSCALLS_PROVIDED
    ret = _open_r(_REENT, file, flags, va_arg(ap, int));
#else
    ret = _open(file, flags, va_arg(ap, int));
#endif
    va_end(ap);
    return ret;
=======
int open(const char *file, int flags, ...) {
  va_list ap;
  int ret;

  va_start(ap, flags);
#ifdef REENTRANT_SYSCALLS_PROVIDED
  ret = _open_r(_REENT, file, flags, va_arg(ap, int));
#else
  ret = _open(file, flags, va_arg(ap, int));
#endif
  va_end(ap);
  return ret;
>>>>>>> master
}

#else /* ! _HAVE_STDC */

<<<<<<< HEAD
int open(file, flags, mode) const char* file;
=======
int open(file, flags, mode) const char *file;
>>>>>>> master
int flags;
int mode;
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
<<<<<<< HEAD
    return _open_r(_REENT, file, flags, mode);
#else
    return _open(file, flags, mode);
=======
  return _open_r(_REENT, file, flags, mode);
#else
  return _open(file, flags, mode);
>>>>>>> master
#endif
}

#endif /* ! _HAVE_STDC */
