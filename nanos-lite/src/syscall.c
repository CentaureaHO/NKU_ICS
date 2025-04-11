#include "syscall.h"
#include "common.h"

static inline _RegSet* sys_none(_RegSet* r)   // 0
{
  SYSCALL_ARG1(r)=1;
  return NULL;
}

static inline _RegSet* sys_write(_RegSet* r)   // 3
{
  char* buf = (char*)SYSCALL_ARG3(r);
  size_t len = SYSCALL_ARG4(r);
  Log("sys_write: %s", buf);
  Log("Write len: %d", len);
  while (len--) _putc(*buf++);

  return NULL;
}

static inline _RegSet* sys_exit(_RegSet* r)   // 4
{
  uintptr_t ret = SYSCALL_ARG2(r);
  Log("Program exit with code %d", ret);
  _halt(ret);
  return NULL;
}

static inline _RegSet* sys_undone(_RegSet* r)   // 999
{
  uintptr_t target = SYSCALL_ARG2(r);

  switch (target) {
    #define X(name, idx, done) \
      case SYS_##name: \
      panic("Not implemented function for %s in navy-apps/libs/libos/src/nanos.c", #name);
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
#define X(name, idx, done) \
  case SYS_##name:         \
    HANDLER_##done(name)
    SYSCALLS
  default:
    panic("Unhandled syscall ID = %d", call_num);
  }

  return NULL;
}
