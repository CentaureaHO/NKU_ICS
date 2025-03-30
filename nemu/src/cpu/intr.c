#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr)
{
    /* TODO: Trigger an interrupt/exception with ``NO''.
     * That is, use ``NO'' to index the IDT.
     */

    rtl_push(&cpu.eflags);
    rtl_push(&ret_addr);
    rtl_li(r0, cpu.cs);
    rtl_push(r0);
}

void dev_raise_intr() {}
