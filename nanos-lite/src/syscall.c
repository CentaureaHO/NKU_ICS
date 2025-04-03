#include "syscall.h"
#include "common.h"

static inline _RegSet* sys_none(_RegSet* r)
{
  SYSCALL_ARG1(r)=1;
  return NULL;
}

static inline _RegSet* sys_exit(_RegSet* r)
{
  uintptr_t ret = SYSCALL_ARG2(r);
  _halt(ret);
  return NULL;
}

_RegSet *do_syscall(_RegSet *r) {
  switch (SYSCALL_ARG1(r)) {
#define HANDLER_Y(name) return sys_##name(r);
#define HANDLER_N(name) TODO()
#define X(name, idx, done) \
  case SYS_##name:         \
    HANDLER_##done(name)  \
    break;
  default:
    panic("Unhandled syscall ID = %d", SYSCALL_ARG1(r));
  }

  return NULL;
}