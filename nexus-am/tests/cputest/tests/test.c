#include "trap.h"

int main() 
{
  int result;
  
  unsigned char test_data = 0xff;
  int dummy1 = 0;
  int dummy2 = 0;
  
  asm volatile(
    "push %%ebp\n\t"
    "movl %%esp, %%ebp\n\t"
    
    "leal %1, %%eax\n\t"
    "movl %%eax, 0xc(%%ebp)\n\t"
    
    "movl 0xc(%%ebp), %%eax\n\t"
    "movl (%%eax), %%eax\n\t"

    "movl %%eax, %0\n\t"
    "pop %%ebp\n\t"
    
    : "=r" (result)
    : "m" (test_data),
      "m" (dummy1),
      "m" (dummy2)
    : "eax", "memory"
  );
  
  // nemu_assert(result == 0xffffffff);
  
  return 0;
}