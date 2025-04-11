#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYSCALLS                                                               \
  X(none, -1, Y)                                                                \
  X(open, 0, N)                                                                \
  X(read, 1, N)                                                                \
  X(write, 2, Y)                                                               \
  X(exit, 3, Y)                                                                \
  X(kill, 4, N)                                                                \
  X(getpid, 5, N)                                                              \
  X(close, 6, N)                                                               \
  X(lseek, 7, N)                                                               \
  X(brk, 8, N)                                                                 \
  X(fstat, 9, N)                                                              \
  X(time, 10, N)                                                               \
  X(signal, 11, N)                                                             \
  X(execve, 12, N)                                                             \
  X(fork, 13, N)                                                               \
  X(link, 14, N)                                                               \
  X(unlink, 15, N)                                                             \
  X(wait, 16, N)                                                               \
  X(times, 17, N)                                                              \
  X(gettimeofday, 18, N)

enum {
#define X(name, idx, done) SYS_##name = idx,
  SYSCALLS
#undef X
};

#endif
