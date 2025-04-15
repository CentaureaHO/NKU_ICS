/* connector for kill */

#include <reent.h>

int kill(pid, sig) int pid;
int sig;
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
<<<<<<< HEAD
    return _kill_r(_REENT, pid, sig);
#else
    return _kill(pid, sig);
=======
  return _kill_r(_REENT, pid, sig);
#else
  return _kill(pid, sig);
>>>>>>> master
#endif
}
