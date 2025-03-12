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
    // TODO();
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

    rtl_li(&t2, id_dest->val);
    rtl_sar(&t0, &t2, &id_src->val);

    rtl_update_PF(&t0);
    rtl_update_ZFSF(&t0, id_dest->width);

    operand_write(id_dest, &t0);

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

    print_asm_template2(shr);
}

make_EHelper(setcc)
{
    uint8_t subcode = decoding.opcode & 0xf;
    rtl_setcc(&t2, subcode);
    operand_write(id_dest, &t2);

    print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not)
{
    TODO();

    print_asm_template1(not);
}