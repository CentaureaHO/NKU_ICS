#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYSCALLS                                                               \
  X(none, 0, Y)                                                                \
  X(open, 1, Y)                                                                \
  X(read, 2, Y)                                                                \
  X(write, 3, Y)                                                               \
  X(exit, 4, Y)                                                                \
  X(kill, 5, N)                                                                \
  X(getpid, 6, N)                                                              \
  X(close, 7, Y)                                                               \
  X(lseek, 8, N)                                                               \
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
};

#endif
