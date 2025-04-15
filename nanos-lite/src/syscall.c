#include "syscall.h"
#include "common.h"
#include "fs.h"

static inline _RegSet *sys_none(_RegSet *r) // 0
{
  SYSCALL_ARG1(r) = 1;
  return NULL;
}

static inline _RegSet *sys_open(_RegSet *r) // 1
{
  const char *path = (const char *)SYSCALL_ARG2(r);
  int flags = SYSCALL_ARG3(r);
  int mode = SYSCALL_ARG4(r);
  SYSCALL_ARG1(r) = fs_open(path, flags, mode);

  return NULL;
}

static inline _RegSet *sys_read(_RegSet *r) // 2
{
  int fd = SYSCALL_ARG2(r);
  char *buf = (char *)SYSCALL_ARG3(r);
  size_t len = SYSCALL_ARG4(r);

  SYSCALL_ARG1(r) = fs_read(fd, buf, len);
  Log("sys_read returns %d", SYSCALL_ARG1(r));

  return NULL;
}

static inline _RegSet *sys_write(_RegSet *r) // 3
{
  int fd = SYSCALL_ARG2(r);
  const char *buf = (const char *)SYSCALL_ARG3(r);
  size_t len = SYSCALL_ARG4(r);

  SYSCALL_ARG1(r) = fs_write(fd, (void *)buf, len);

  return NULL;
}

static inline _RegSet *sys_exit(_RegSet *r) // 4
{
  uintptr_t ret = SYSCALL_ARG2(r);
  Log("Program exit with code %d", ret);
  _halt(ret);
  return NULL;
}

static inline _RegSet *sys_close(_RegSet *r) // 5
{
  int fd = SYSCALL_ARG2(r);
  SYSCALL_ARG1(r) = fs_close(fd);

  return NULL;
}

static inline _RegSet *sys_lseek(_RegSet *r) // 6
{
  int fd = SYSCALL_ARG2(r);
  off_t offset = SYSCALL_ARG3(r);
  int whence = SYSCALL_ARG4(r);

  SYSCALL_ARG1(r) = fs_lseek(fd, offset, whence);

  return NULL;
}

static inline _RegSet *sys_brk(_RegSet *r) // 9
{
  SYSCALL_ARG1(r) = 0;
  return NULL;
}

static inline _RegSet *sys_undone(_RegSet *r) // 999
{
  uintptr_t target = SYSCALL_ARG2(r);

  switch (target) {
#define X(name, idx, done)                                                     \
  case SYS_##name:                                                             \
    panic(                                                                     \
        "Not implemented function for %s in navy-apps/libs/libos/src/nanos.c", \
        #name);
    SYSCALLS
#undef X
  default:
    panic("Unhandled syscall ID = %d", target);
  }

  return NULL;
}

_RegSet *do_syscall(_RegSet *r) {

  int call_num = SYSCALL_ARG1(r);

  Log("syscall: %d", call_num);
  switch (call_num) {
#define HANDLER_Y(name) return sys_##name(r);
#define HANDLER_N(name) panic("Unhandled syscall ID = %d", call_num);
#define X(name, idx, done)                                                     \
  case SYS_##name:                                                             \
    HANDLER_##done(name)
    SYSCALLS
  default:
    panic("Unhandled syscall ID = %d", call_num);
  }

  return NULL;
}
