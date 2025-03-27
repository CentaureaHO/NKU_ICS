#include "cpu/exec.h"

make_EHelper(mov)
{
    // Flags Affected: None

    rtl_li(r0, id_src->val);
    rtl_sext(r0, r0, id_src->width);

    operand_write(id_dest, r0);
    print_asm_template2(mov);
}

make_EHelper(push)
{
    // Flags Affected: None

    rtl_li(r0, id_dest->val);
    rtl_push(r0);

    print_asm_template1(push);
}

make_EHelper(pop)
{
    // Flags Affected: None

    rtl_pop(r0);
    rtl_sr(id_dest->reg, id_dest->width, r0);

    print_asm_template1(pop);
}

make_EHelper(pusha)
{
    TODO();

    print_asm("pusha");
}

make_EHelper(popa)
{
    TODO();

    print_asm("popa");
}

make_EHelper(leave)
{
    // Flags Affected: None

    rtl_lr(r0, R_EBP, 4);
    rtl_sr(R_ESP, 4, r0);

    rtl_pop(r0);
    rtl_sr(R_EBP, 4, r0);

    print_asm("leave");
}

make_EHelper(cltd)
{
    // Flags Affected: None

    if (decoding.is_operand_size_16) {
        rtl_lr(r0, R_AX, 2);
        rtl_msb(r0, r0, 2);

        rtl_not(r0);               // t0: 0x00000000 -> 0xffffffff | 0x00000001 -> 0xfffffffe
        rtl_addi(r0, r0, 0x1);     // t0: 0xffffffff -> 0x00000000 | 0xfffffffe -> 0xffffffff
        rtl_andi(r0, r0, 0xffff);  // t0: 0x00000000 | 0x0000ffff
        rtl_sr(R_DX, 2, r0);
    }
    else
    {
        rtl_lr(r0, R_EAX, 4);
        rtl_msb(r0, r0, 4);

        rtl_not(r0);
        rtl_addi(r0, r0, 0x1);
        rtl_sr(R_EDX, 4, r0);
    }

    print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl)
{
    if (decoding.is_operand_size_16) {
        rtl_lr(r0, R_AL, 1);
        rtl_sext(r0, r0, 1);
        rtl_sr(R_AX, 2, r0);
    }
    else
    {
        rtl_lr(r0, R_AX, 2);
        rtl_sext(r0, r0, 2);
        rtl_sr(R_EAX, 4, r0);
    }

    print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx)
{
    // Flags Affected: None

    id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
    rtl_sext(&t2, &id_src->val, id_src->width);
    operand_write(id_dest, &t2);
    print_asm_template2(movsx);
}

make_EHelper(movzx)
{
    // Flags Affected: None

    id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
    operand_write(id_dest, &id_src->val);
    print_asm_template2(movzx);
}

make_EHelper(lea)
{
    // Flags Affected: None

    rtl_li(&t2, id_src->addr);
    operand_write(id_dest, &t2);
    print_asm_template2(lea);
}