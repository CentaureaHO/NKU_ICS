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

    uint32_t gate_addr = cpu.idtr.base + NO * 8;
    uint16_t tar_cs = vaddr_read(gate_addr + 2, 2);
    uint32_t tar_offset_low = vaddr_read(gate_addr, 2);
    uint32_t tar_offset_high = vaddr_read(gate_addr + 6, 2) << 16;

    cpu.cs = tar_cs;
    cpu.eip = tar_offset_low | tar_offset_high;

    print_asm("intr %#x", NO);
}

void dev_raise_intr() {}
