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
        // _halt(SYSCALL_ARG1(r));
        Log("Arg 1: %d", SYSCALL_ARG1(r));
        Log("Arg 2: %d", SYSCALL_ARG2(r));
        Log("Arg 3: %d", SYSCALL_ARG3(r));
        Log("Arg 4: %d", SYSCALL_ARG4(r));
        _halt(SYSCALL_ARG1(r));
        break;
      default:
        panic("Unhandled syscall ID = %d", a[0]);
    }

    return NULL;
}
