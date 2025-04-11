#include "syscall.h"
#include "common.h"

static inline _RegSet* sys_none(_RegSet* r)   // 0
{
  SYSCALL_ARG1(r)=1;
  return NULL;
}

static inline _RegSet* sys_write(_RegSet* r)   // 3
{
  panic("sys_write: %d", SYSCALL_ARG1(r));
  return NULL;
}

static inline _RegSet* sys_exit(_RegSet* r)   // 4
{
  uintptr_t ret = SYSCALL_ARG2(r);
  _halt(ret);
  return NULL;
}

_RegSet *do_syscall(_RegSet *r) {

  int call_num = SYSCALL_ARG1(r);   // ? why
                                    // I find that the sys call num is put in ebx

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
