#ifndef __SYSCALL_H__
#define __SYSCALL_H__

<<<<<<< HEAD
enum
{
    SYS_none,
    SYS_open,
    SYS_read,
    SYS_write,
    SYS_exit,
    SYS_kill,
    SYS_getpid,
    SYS_close,
    SYS_lseek,
    SYS_brk,
    SYS_fstat,
    SYS_time,
    SYS_signal,
    SYS_execve,
    SYS_fork,
    SYS_link,
    SYS_unlink,
    SYS_wait,
    SYS_times,
    SYS_gettimeofday
=======
#define SYSCALLS                                                               \
  X(none, 0, Y)                                                                \
  X(open, 1, Y)                                                                \
  X(read, 2, Y)                                                                \
  X(write, 3, Y)                                                               \
  X(exit, 4, Y)                                                                \
  X(kill, 5, N)                                                                \
  X(getpid, 6, N)                                                              \
  X(close, 7, Y)                                                               \
  X(lseek, 8, Y)                                                               \
  X(brk, 9, Y)                                                                 \
  X(fstat, 10, N)                                                              \
  X(time, 11, N)                                                               \
  X(signal, 12, N)                                                             \
  X(execve, 13, N)                                                             \
  X(fork, 14, N)                                                               \
  X(link, 15, N)                                                               \
  X(unlink, 16, N)                                                             \
  X(wait, 17, N)                                                               \
  X(times, 18, N)                                                              \
  X(gettimeofday, 19, N)                                                       \
  X(undone, 999, Y)

enum {
#define X(name, idx, done) SYS_##name = idx,
  SYSCALLS
#undef X
>>>>>>> master
};

#endif
