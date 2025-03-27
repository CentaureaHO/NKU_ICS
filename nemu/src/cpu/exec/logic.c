#include "cpu/exec.h"

make_EHelper(test)
{
    // OF = 0, CF = 0; SF, ZF, and PF as described in Appendix C

    rtl_and(&t0, &id_dest->val, &id_src->val);

    rtl_set_OF(&tzero);
    rtl_set_CF(&tzero);

    rtl_update_PF(&t0);
    rtl_update_ZFSF(&t0, id_dest->width);

    print_asm_template2(test);
}

make_EHelper(and)
{
    // CF = 0, OF = 0; PF, SF, and ZF as described in Appendix C

    // Log("id_dest->val = 0x%x, id_src->val = 0x%x", id_dest->val, id_src->val);

    rtl_and(&t0, &id_dest->val, &id_src->val);
    operand_write(id_dest, &t0);

    rtl_set_CF(&tzero);
    rtl_set_OF(&tzero);
    rtl_update_PF(&t0);
    rtl_update_ZFSF(&t0, id_dest->width);

    print_asm_template2(and);
}

make_EHelper (xor)
{
    // CF = 0, OF = 0; SF, ZF, and PF as described in Appendix C; AF is undefined

    rtl_xor(&t0, &id_dest->val, &id_src->val);
    rtl_set_CF(&tzero);
    rtl_set_OF(&tzero);

    rtl_update_ZFSF(&t0, id_dest->width);
    rtl_update_PF(&t0);

    operand_write(id_dest, &t0);

    print_asm_template2 (xor);
}

make_EHelper(or)
{
    // OF ← 0, CF ← 0; SF, ZF, and PF as described in Appendix C; AF is undefined
    rtl_or(&t0, &id_dest->val, &id_src->val);

    rtl_set_OF(&tzero);
    rtl_set_CF(&tzero);

    rtl_update_PF(&t0);
    rtl_update_ZFSF(&t0, id_dest->width);

    operand_write(id_dest, &t0);

    print_asm_template2(or);
}

make_EHelper(sar)
{
    // unnecessary to update CF and OF in NEMU
    // why?
    // OF for single shifts; OF is undefined for multiple shifts; CF, ZF, PF, and SF as described in Appendix C

    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);

    /*
    rtl_get_OF(r2);
    rtl_subi(r3, r1, 0x1);
    Log("Got t3 = 0x%x in rtl_subi", t3);
    rtl_neq0(r3, r3);
    rtl_and(r2, r2, r3);
    rtl_set_OF(r2);
    */
    rtl_set_OF(disable);

    rtl_subi(r2, r1, 0x1);
    rtl_mtb(r2, r0, t2);  // r2 = ((dest >> (src - 1)) & 0x1)
    // Log("Got t2 = 0x%x in rtl_mtb", t2);
    /*
    rtl_neq0(r3, r1);
    rtl_and(r2, r2, r3);
    rtl_get_CF(r3);
    rtl_or(r3, r3, r2);
    */
    rtl_set_CF(r2);

    rtl_sar(r0, r0, r1);

    rtl_update_PFZFSF(r0, id_dest->width);

    operand_write(id_dest, r0);

    /*
    rtl_li(&t2, id_dest->val);
    rtl_sar(&t0, &t2, &id_src->val);

    rtl_update_PF(&t0);
    rtl_update_ZFSF(&t0, id_dest->width);

    operand_write(id_dest, &t0);
    */

    print_asm_template2(sar);
}

make_EHelper(shl)
{
    // unnecessary to update CF and OF in NEMU
    /*
        OF for single shifts; OF is undefined for multiple shifts; CF, ZF, PF,
        and SF as described in Appendix C
    */

    rtl_li(&t0, id_dest->val);
    rtl_shl(&t0, &t0, &id_src->val);

    rtl_update_PF(&t0);
    rtl_update_ZFSF(&t0, id_dest->width);

    operand_write(id_dest, &t0);

    print_asm_template2(shl);
}

make_EHelper(shr)
{
    // unnecessary to update CF and OF in NEMU
    // same as shl

    rtl_li(&t0, id_dest->val);
    rtl_shr(&t0, &t0, &id_src->val);

    rtl_update_PF(&t0);
    rtl_update_ZFSF(&t0, id_dest->width);

    operand_write(id_dest, &t0);

    print_asm_template2(shr);
}

make_EHelper(setcc)
{
    // Flags Affected: None

    uint8_t subcode = decoding.opcode & 0xf;
    rtl_setcc(&t2, subcode);
    operand_write(id_dest, &t2);

    print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not)
{
    // Flags Affected: None

    rtl_mv(&t0, &id_dest->val);
    rtl_not(&t0);
    operand_write(id_dest, &t0);

    print_asm_template1(not);
}

make_EHelper(rol)
{
    // Flags Affected: OF only for single rotates; OF is undefined for multi-bit rotates; CF as described above

    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);

    rtl_rol(r0, r0, r1, id_dest->width);
    operand_write(id_dest, r0);
    rtl_li(r2, 0x1);
    rtl_and(r2, r0, r2);
    rtl_set_CF(r2);

    // OF = r1 == 1 && (r2 != sign(r0));
    rtl_eqi(r3, r1, 0x1);
    rtl_msb(r0, r0, id_dest->width);
    // rtl_neq(r0, r0, r2);
    rtl_sub(r0, r0, r2);
    rtl_neq0(r0, r0);
    rtl_and(r3, r3, r0);

    rtl_set_OF(r3);

    print_asm_template2(rol);
}

make_EHelper(ror)
{
    // Flags Affected: OF only for single rotates; OF is undefined for multi-bit rotates; CF as described above
    
    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);

    rtl_ror(r0, r0, r1, id_dest->width);
    operand_write(id_dest, r0);
    rtl_msb(r2, r0, id_dest->width);
    rtl_set_CF(r2);

    // OF = r1 == 1 && (r2 != sign_next(r0));
    rtl_mtb(r3, r3, (id_dest->width << 3) - 1);
    // rtl_neq(r2, r2, r3);
    rtl_sub(r2, r2, r3);
    rtl_neq0(r2, r2);
    rtl_eqi(r3, r1, 0x1);
    rtl_and(r3, r3, r2);

    rtl_set_OF(r3);

    print_asm_template2(ror);
}
