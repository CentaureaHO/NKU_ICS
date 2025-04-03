#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr)
{
    /* TODO: Trigger an interrupt/exception with ``NO''.
     * That is, use ``NO'' to index the IDT.
     */

    rtl_push(&cpu.eflags);
    rtl_li(r0, cpu.cs);
    rtl_push(r0);
    rtl_push(&ret_addr);

    uint32_t gate_addr = cpu.idtr.base + NO * 8;
    uint32_t tar_eip_low = vaddr_read(gate_addr, 4) & 0xffff;
    uint32_t tar_eip_high = vaddr_read(gate_addr + 4, 4) & 0xffff0000;

    decoding.jmp_eip = tar_eip_low | tar_eip_high;
    decoding.is_jmp = 1;

    print_asm("intr %#x", NO);
}

void dev_raise_intr() {}
