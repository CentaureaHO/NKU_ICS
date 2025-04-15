/* connector for fork */

#include <reent.h>

<<<<<<< HEAD
int fork()
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
    return _fork_r(_REENT);
#else
    return _fork();
=======
int fork() {
#ifdef REENTRANT_SYSCALLS_PROVIDED
  return _fork_r(_REENT);
#else
  return _fork();
>>>>>>> master
#endif
}
