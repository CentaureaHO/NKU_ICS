#ifndef __TRAP_H__
#define __TRAP_H__

#include <am.h>
#include <klib.h>

<<<<<<< HEAD
__attribute__((noinline)) void nemu_assert(int cond)
{
    if (!cond) _halt(1);
=======
__attribute__((noinline)) void nemu_assert(int cond) {
  if (!cond)
    _halt(1);
>>>>>>> master
}

#endif
