#include "cpu/exec.h"

void diff_test_skip_qemu();
void diff_test_skip_nemu();
void raise_intr(uint8_t NO, vaddr_t ret_addr);

make_EHelper(lidt)
{
    rtl_lm(r0, &id_dest->addr, 2);
    rtl_sr_idtr_w(r0);

    rtl_li(r0, id_dest->addr);
    rtl_addi(r0, r0, 2);
    rtl_lm(r0, r0, 4);
    if (decoding.is_operand_size_16)
    {
        rtl_li(r1, 0x00ffffff);
        rtl_and(r0, r0, r1);
    }
    rtl_sr_idtr_b(r0);

    print_asm_template1(lidt);
}

make_EHelper(mov_r2cr)
{
    TODO();

    print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r)
{
    TODO();

    print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#ifdef DIFF_TEST
    diff_test_skip_qemu();
#endif
}

make_EHelper(int)
{
    Log("int %s", id_dest->str);
    Log("id_dest->val = %d", id_dest->val);
    
    Log("id_dest->imm = %d", id_dest->imm);
    raise_intr(id_dest->val, decoding.seq_eip);

    print_asm("int %s", id_dest->str);

#ifdef DIFF_TEST
    diff_test_skip_nemu();
#endif
}

make_EHelper(into)
{
    TODO();

    print_asm("into %s", id_dest->str);

#ifdef DIFF_TEST
    diff_test_skip_nemu();
#endif
}

make_EHelper(iret)
{
    TODO();

    print_asm("iret");
}

uint32_t pio_read(ioaddr_t, int);
void     pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in)
{
    t0 = pio_read(id_src->val, id_dest->width);
    rtl_sr(R_EAX, id_dest->width, r0);

    print_asm_template2(in);

#ifdef DIFF_TEST
    diff_test_skip_qemu();
#endif
}

make_EHelper(out)
{
    rtl_li(r0, id_src->val);
    rtl_li(r1, id_dest->val);
    rtl_li(r2, id_dest->width);
    pio_write(t1, t2, t0);

    print_asm_template2(out);

#ifdef DIFF_TEST
    diff_test_skip_qemu();
#endif
}
