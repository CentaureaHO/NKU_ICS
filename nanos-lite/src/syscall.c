#include "syscall.h"
#include "common.h"

static inline _RegSet* sys_none(_RegSet* r)
{
  return NULL;
}

static inline _RegSet* sys_exit(_RegSet* r)
{
  return NULL;
}

_RegSet *do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);

  switch (a[0]) {
#define HANDLER_Y(name) return sys_##name(r);
#define HANDLER_N(name) TODO()
#define X(name, idx, done) \
  case SYS_##name:         \
    HANDLER_##done(name)
  default:
    panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}