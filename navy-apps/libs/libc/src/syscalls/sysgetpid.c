/* connector for getpid */

#include <reent.h>

<<<<<<< HEAD
int getpid()
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
    return _getpid_r(_REENT);
#else
    return _getpid();
=======
int getpid() {
#ifdef REENTRANT_SYSCALLS_PROVIDED
  return _getpid_r(_REENT);
#else
  return _getpid();
>>>>>>> master
#endif
}
