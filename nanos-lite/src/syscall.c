#include "syscall.h"
#include "common.h"

_RegSet *do_syscall(_RegSet *r) {
    uintptr_t a[4];
    a[0] = SYSCALL_ARG1(r);

    switch (a[0]) 
    {
      case SYS_none:
        SYSCALL_ARG1(r) = 1; 
        break;
      case SYS_exit:
      {
        int ret = SYSCALL_ARG2(r);
        _halt(ret);
        break;
      }
      default:
        panic("Unhandled syscall ID = %d", a[0]);
    }

    return NULL;
}
