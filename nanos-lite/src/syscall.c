#include "syscall.h"
#include "common.h"

_RegSet *do_syscall(_RegSet *r) {
    uintptr_t a[4];
    a[0] = SYSCALL_ARG1(r);

    switch (a[0]) 
    {
      case SYS_none:  // 0
        SYSCALL_ARG1(r) = 1; 
        break;
      case SYS_write: // 3

      case SYS_exit:  // 4
      {
        a[1] = SYSCALL_ARG2(r);
        _halt(a[1]);
        break;
      }
      default:
        panic("Unhandled syscall ID = %d", a[0]);
    }

    return NULL;
}
