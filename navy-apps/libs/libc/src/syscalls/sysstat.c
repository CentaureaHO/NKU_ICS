/* connector for stat */

#include <reent.h>
#include <unistd.h>

<<<<<<< HEAD
int          stat(file, pstat) char* file;
struct stat* pstat;
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
    return _stat_r(_REENT, file, pstat);
#else
    return _stat(file, pstat);
=======
int stat(file, pstat) char *file;
struct stat *pstat;
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
  return _stat_r(_REENT, file, pstat);
#else
  return _stat(file, pstat);
>>>>>>> master
#endif
}
