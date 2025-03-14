#include "cpu/exec.h"

make_EHelper(add)
{
    // OF, SF, ZF, AF, CF, and PF as described in Appendix C

    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);
    if (id_src->width == 1 && id_dest->width > 1) rtl_sext(r1, r1, id_src->width);

    /* r0: add src0, r1: add src1 */

    // update AF:
    rtl_andi(r2, r0, 0xf);   // low 4 bits of src0 -> r2
    rtl_andi(r3, r1, 0xf);   // low 4 bits of src1 -> r3
    rtl_add(r2, r2, r3);     // add res of low 4 bits -> r2
    rtl_andi(r2, r2, 0x10);  // check if carry of low 4 bits -> r2
    rtl_set_AF(r2);

    rtl_add(r2, r0, r1);  // add src0 and src1 -> r2

    /* r0: add src0, r1: add src1, r2: add res */

    // write back
    operand_write(id_dest, r2);

    // update PF, ZF, SF:
    rtl_update_PFZFSF(r2, id_dest->width);

    // update CF: carry means res < src0 && res < src1
    rtl_sltu(r3, r2, r0);
    rtl_set_CF(r3);

    // update OF: overflow only happens when src0 and src1 have the same sign,
    //                                  while res has different sign
    rtl_xor(r2, r0, r2);  // sign_bit(r2) = 1 if src0 has different sign with res else 0
    rtl_xor(r3, r0, r1);  // sign_bit(r3) = 0 if src0 has the same sign with src1 else 1
    rtl_not(r3);          // sign_bit(r3) = 1 if src0 has the same sign with src1 else 0
    rtl_and(r1, r2, r3);  // if sign_bit(r1) -> overflow
    rtl_msb(r0, r1, id_dest->width);
    rtl_set_OF(r0);

    print_asm_template2(add);
}

make_EHelper(sub)
{
    // OF, SF, ZF, AF, PF, and CF as described in Appendix C

    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);
    if (id_src->width == 1 && id_dest->width > 1) rtl_sext(r1, r1, id_src->width);

    /* r0: minuend, r1: subtrahend */

    // update AF: AF = (low4(minuend) < low4(subtrahend)) ? 1 : 0
    rtl_andi(r2, r0, 0xf);  // low 4 bits of minuend -> r2
    rtl_andi(r3, r1, 0xf);  // low 4 bits of subtrahend -> r3
    rtl_sltu(r2, r2, r3);   // low 4 bits of minuend < low 4 bits of subtrahend -> r2
    rtl_set_AF(r2);

    rtl_sub(r2, r0, r1);  // minuend - subtrahend -> r2

    /* r0: minuend, r1: subtrahend, r2: sub res */

    // write back
    operand_write(id_dest, r2);

    // update PF, ZF, SF:
    rtl_update_PFZFSF(r2, id_dest->width);

    // update CF: carry means minuend < subtrahend
    rtl_sltu(r3, r0, r1);
    rtl_set_CF(r3);

    // update OF: overflow only happens when minuend and subtrahend have different sign,
    //                                  while res has different sign with minuend
    rtl_xor(r2, r0, r2);  // sign_bit(r2) = 1 if minuend has different sign with res else 0
    rtl_xor(r3, r0, r1);  // sign_bit(r3) = 0 if minuend has the same sign with subtrahend else 1
    rtl_and(r1, r2, r3);  // if sign_bit(r1) -> overflow
    rtl_msb(r0, r1, id_dest->width);
    rtl_set_OF(r0);

    print_asm_template2(sub);
}

make_EHelper(cmp)
{
    // OF, SF, ZF, AF, PF, and CF as described in Appendix C

    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);
    if (id_src->width == 1 && id_dest->width > 1) rtl_sext(r1, r1, id_src->width);

    /* r0: minuend, r1: subtrahend */

    // update AF: AF = (low4(minuend) < low4(subtrahend)) ? 1 : 0
    rtl_andi(r2, r0, 0xf);  // low 4 bits of minuend -> r2
    rtl_andi(r3, r1, 0xf);  // low 4 bits of subtrahend -> r3
    rtl_sltu(r2, r2, r3);   // low 4 bits of minuend < low 4 bits of subtrahend -> r2
    rtl_set_AF(r2);

    rtl_sub(r2, r0, r1);  // minuend - subtrahend -> r2

    /* r0: minuend, r1: subtrahend, r2: sub res */

    // update PF, ZF, SF:
    rtl_update_PFZFSF(r2, id_dest->width);

    // update CF: carry means minuend < subtrahend
    rtl_sltu(r3, r0, r1);
    rtl_set_CF(r3);

    // update OF: overflow only happens when minuend and subtrahend have different sign,
    //                                  while res has different sign with minuend
    rtl_xor(r2, r0, r2);  // sign_bit(r2) = 1 if minuend has different sign with res else 0
    rtl_xor(r3, r0, r1);  // sign_bit(r3) = 0 if minuend has the same sign with subtrahend else 1
    rtl_and(r1, r2, r3);  // if sign_bit(r1) -> overflow
    rtl_msb(r0, r1, id_dest->width);
    rtl_set_OF(r0);

    print_asm_template2(cmp);
}

make_EHelper(inc)
{
    // OF, SF, ZF, AF, and PF as described in Appendix C

    rtl_li(r0, id_dest->val);
    rtl_addi(r1, r0, 0x1);

    /* r0: inc src, r1: inc res */

    // write back
    operand_write(id_dest, r1);

    // update PF, ZF, SF:
    rtl_update_PFZFSF(r1, id_dest->width);

    // update AF: AF = (low4(src) == 0xf) ? 1 : 0, or AF = (low4(res) == 0) ? 1 : 0
    rtl_andi(r2, r0, 0xf);  // low 4 bits of src -> r2
    rtl_eqi(r3, r2, 0xf);   // low 4 bits of src == 0xf -> r3
    rtl_set_AF(r3);

    // update OF: overflow only happens when sign_bit(src) == 0 && sign_bit(res) == 1
    rtl_msb(r2, r0, id_dest->width);
    rtl_msb(r3, r1, id_dest->width);
    rtl_eq0(r2, r2);       // sign_bit(src) == 0 -> r2
    rtl_eqi(r3, r3, 0x1);  // sign_bit(res) == 1 -> r3
    rtl_and(r1, r2, r3);
    rtl_set_OF(r1);

    print_asm_template1(inc);
}

make_EHelper(dec)
{
    // OF, SF, ZF, AF, and PF as described in Appendix C.

    rtl_li(r0, id_dest->val);
    rtl_subi(r1, r0, 0x1);

    /* r0: dec src, r1: dec res */

    // write back
    operand_write(id_dest, r1);

    // update PF, ZF, SF:
    rtl_update_PFZFSF(r1, id_dest->width);

    // update AF: AF = (low4(src) == 0) ? 1 : 0, or AF = (low4(res) == 0xf) ? 1 : 0
    rtl_andi(r2, r0, 0xf);  // low 4 bits of src -> r2
    rtl_eq0(r3, r2);        // low 4 bits of src == 0 -> r3
    rtl_set_AF(r3);

    // update OF: overflow only happens when sign_bit(src) == 1 && sign_bit(res) == 0
    rtl_msb(r2, r0, id_dest->width);
    rtl_msb(r3, r1, id_dest->width);
    rtl_eqi(r2, r2, 0x1);  // sign_bit(src) == 1 -> r2
    rtl_eq0(r3, r3);       // sign_bit(res) == 0 -> r3
    rtl_and(r1, r2, r3);
    rtl_set_OF(r1);

    print_asm_template1(dec);
}

make_EHelper(neg)
{
    // The carry flag is set to 1, unless the operand is zero, in which case the
    // carry flag is cleared to 0.
    // CF as described above; OF, SF, ZF, and PF as described in Appendix C
    TODO();

    print_asm_template1(neg);
}

make_EHelper(adc)
{
    // OF, SF, ZF, AF, CF, and PF as described in Appendix C
    
    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);
    if (id_src->width == 1 && id_dest->width > 1) rtl_sext(r1, r1, id_src->width);

    uint32_t _dest = t0, _src = t1;
    uint32_t* dest = &_dest;
    uint32_t* src  = &_src;

    rtl_add(r0, dest, src);     // r0 = dest + src
    rtl_sltu(r1, r0, dest);     // r1 = (dest + src) < dest

    rtl_get_CF(r2);
    rtl_add(r3, r0, r2);        // r3 = dest + src + CF
    rtl_sltu(r2, r3, r0);       // r2 = (dest + src + CF) < (dest + src)
    
    /* r0: dest + src, r1: (dest + src) < dest, 
       r2: (dest + src + CF) < (dest + src), r3: dest + src + CF */

    uint32_t tmp0 = 0, tmp1 = 0;
    uint32_t* p0 = &tmp0;
    uint32_t* p1 = &tmp1;
    rtl_andi(r0, dest, 0xf);
    rtl_andi(p0, src, 0xf);
    rtl_add(p0, r0, p0);
    rtl_get_CF(p1);
    rtl_add(p0, p0, p1);
    rtl_andi(p1, p0, 0x10);
    rtl_set_AF(p1);

    rtl_or(r1, r1, r2);         // r1 = r1 || r2
    rtl_set_CF(r1);             // CF = r1

    rtl_update_PFZFSF(r3, id_dest->width);

    operand_write(id_dest, r3);

    rtl_xor(r0, dest, src);     // r0 = dest ^ src
    rtl_not(r0);                // r0 = ~(dest ^ src)
    rtl_xor(r1, dest, r3);      // r1 = dest ^ (dest + src + CF)
    rtl_and(r0, r0, r1);        // r0 = ~(dest ^ src) & (dest ^ (dest + src + CF))
    rtl_msb(r0, r0, id_dest->width);
    rtl_set_OF(r0);             // OF = msb(~(dest ^ src) & (dest ^ (dest + src + CF)))

    print_asm_template2(adc);
}

make_EHelper(sbb)
{
    // OF, SF, ZF, AF, PF, and CF as described in Appendix C
    
    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);
    if (id_src->width == 1 && id_dest->width > 1) rtl_sext(r1, r1, id_src->width);

    uint32_t _dest = t0, _src = t1;
    uint32_t* dest = &_dest;
    uint32_t* src  = &_src;

    rtl_sub(r0, dest, src);     // r0 = dest - src
    rtl_sltu(r1, dest, r0);     // r1 = dest < (dest - src)

    rtl_get_CF(r2);
    rtl_sub(r3, r0, r2);        // r3 = dest - src - CF
    rtl_sltu(r2, r0, r3);       // r2 = (dest - src) < (dest - src - CF)
    
    /* r0: dest - src, r1: dest < (dest - src), 
       r2: (dest - src) < (dest - src - CF), r3: dest - src - CF */

    uint32_t tmp0 = 0, tmp1 = 0;
    uint32_t* p0 = &tmp0;
    uint32_t* p1 = &tmp1;
    rtl_andi(r0, dest, 0xf);
    rtl_andi(p0, src, 0xf);
    rtl_get_CF(p1);
    rtl_add(p0, p0, p1);
    rtl_sltu(p1, r0, p0);
    rtl_set_AF(p1);

    rtl_or(r1, r1, r2);         // r1 = r1 || r2
    rtl_set_CF(r1);             // CF = r1

    rtl_update_PFZFSF(r3, id_dest->width);

    operand_write(id_dest, r3);

    rtl_xor(r0, dest, src);     // r0 = dest ^ src
    rtl_xor(r1, dest, r3);      // r1 = dest ^ (dest - src - CF)
    rtl_and(r0, r0, r1);        // r0 = (dest ^ src) & (dest ^ (dest - src - CF))
    rtl_msb(r0, r0, id_dest->width);
    rtl_set_OF(r0);             // OF = msb((dest ^ src) & (dest ^ (dest - src - CF)))

    print_asm_template2(sbb);
}

make_EHelper(mul)
{
    /*
        AL * r/m8 → AX:
            AH != 0 → OF = 1, CF = 1
            AH == 0 → OF = 0, CF = 0
        AX * r/m16 → DX:AX:
            DX != 0 → OF = 1, CF = 1
            DX == 0 → OF = 0, CF = 0
        EAX * r/m32 → EDX:EAX:
            EDX != 0 → OF = 1, CF = 1
            EDX == 0 → OF = 0, CF = 0
     */
    // OF and CF as described above; SF, ZF, AF, PF, and CF are undefined

    rtl_lr(r0, R_EAX, id_dest->width);
    rtl_li(r1, id_dest->val);
    rtl_mul(r1, r0, r1, r0);

    switch (id_dest->width)
    {
        case 1:
            rtl_sr_w(R_AX, r0);
            rtl_lr_b(r0, R_AH);
            break;
        case 2:
            rtl_sr_w(R_AX, r0);
            rtl_shri(r0, r0, 16);
            rtl_sr_w(R_DX, r0);
            rtl_lr_w(r0, R_DX);
            break;
        case 4:
            rtl_sr_l(R_EDX, r1);
            rtl_sr_l(R_EAX, r0);
            rtl_lr_l(r0, R_EDX);
            break;
        default: assert(0);
    }

    rtl_neq0(r1, r0);
    rtl_set_CF(r1);
    rtl_set_OF(r1);

    print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1)
{
    /*
        clear OF and CF while:
            AL * r/m8 → AX: AL  ==  sext(AL, 2)
            AX * r/m16 → DX:AX: AX  ==  sext(AX, 4)
            EAX * r/m32 → EDX:EAX: EAX ==  sext(EAX, 4)
     */
    // OF and CF as described above; SF, ZF, AF, PF are undefined

    Log("imul1 is not implemented for setting eflags");
    rtl_set_SF(disable);
    rtl_set_ZF(disable);
    rtl_set_AF(disable);
    rtl_set_PF(disable);

    rtl_lr(r0, R_EAX, id_dest->width);
    rtl_li(r1, id_dest->val);
    rtl_imul(r1, r0, r1, r0);

    switch (id_dest->width)
    {
        case 1:
            rtl_sr_w(R_AX, r0);
            rtl_sext(r2, r0, 1);
            // rtl_eq(r3, r0, r2);
            rtl_sub(r3, r0, r2);
            rtl_eq0(r3, r3);
            rtl_not(r3);
            rtl_set_CF(r3);
            rtl_set_OF(r3);
            break;
        case 2:
            rtl_sr_w(R_AX, r0);
            rtl_shri(r2, r0, 16);
            rtl_sr_w(R_DX, r2);
            rtl_sari(r3, r0, 15);
            // rtl_eq(r3, r2, r3);
            rtl_sub(r3, r2, r3);
            rtl_eq0(r3, r3);
            rtl_not(r3);
            rtl_set_CF(r3);
            rtl_set_OF(r3);
            break;
        case 4:
            rtl_sr_l(R_EDX, r1);
            rtl_sr_l(R_EAX, r0);
            rtl_sari(r3, r0, 31);
            // rtl_eq(r3, r1, r3);
            rtl_sub(r3, r1, r3);
            rtl_eq0(r3, r3);
            rtl_not(r3);
            rtl_set_CF(r3);
            rtl_set_OF(r3);
            break;
        default: assert(0);
    }

    print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2)
{
    /*
        clear OF and CF while:
            r16 * r/m16 → r16:  exactly fits within r16
            r32 * r/m32 → r32:  exactly fits within r32
     */
    // OF and CF as described above; SF, ZF, AF, PF, and CF are undefined

    Log("imul2 is not implemented for setting eflags");

    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);
    rtl_sext(r2, r0, id_dest->width);
    rtl_sext(r3, r1, id_src->width);

    rtl_imul(r1, r0, r2, r3);

    operand_write(id_dest, r0);

    print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3)
{
    /*
        clear OF and CF while:
            r/m16 * imm16 → r16:    exactly fits within r16
            r/m32 * imm32 → r32:    exactly fits within r32
     */
    // OF and CF as described above; SF, ZF, AF, PF, and CF are undefined

    Log("imul3 is not implemented for setting eflags");

    rtl_li(r0, id_dest->val);
    rtl_li(r1, id_src->val);
    rtl_li(r2, id_src2->val);
    rtl_sext(r0, r0, id_dest->width);
    rtl_sext(r1, r1, id_src->width);
    rtl_sext(r2, r2, id_src->width);

    rtl_imul(r1, r0, r1, r2);

    operand_write(id_dest, r0);

    print_asm_template3(imul);
}

make_EHelper(div)
{
    // OF, SF, ZF, AR(AF), PF, CF are undefined.

    rtl_li(r0, id_dest->val);

    switch (id_dest->width)
    {
        case 1:
            rtl_mv(r2, rzero);
            rtl_lr_w(r1, R_AX);
            break;
        case 2:
            rtl_lr_w(r1, R_AX);
            rtl_lr_w(r2, R_DX);
            rtl_shli(r2, r2, 16);
            rtl_or(r1, r1, r2);
            rtl_mv(r2, rzero);
            break;
        case 4:
            rtl_lr_l(r1, R_EAX);
            rtl_lr_l(r2, R_EDX);
            break;
        default: assert(0);
    }

    rtl_div(r1, r0, r2, r1, r0);

    rtl_sr(R_EAX, id_dest->width, r1);
    if (id_dest->width == 1)
        rtl_sr_b(R_AH, r0);
    else
        rtl_sr(R_EDX, id_dest->width, r0);

    print_asm_template1(div);
}

make_EHelper(idiv)
{
    // OF, SF, ZF, AR(AF), PF, CF are undefined.

    rtl_sext(r0, &id_dest->val, id_dest->width);
    switch (id_dest->width)
    {
        case 1:
            rtl_lr_w(r1, R_AX);
            rtl_sext(r1, r1, 2);
            rtl_msb(r2, r1, 4);
            rtl_sub(r2, rzero, r2);
            break;
        case 2:
            rtl_lr_w(r1, R_AX);
            rtl_lr_w(r2, R_DX);
            rtl_shli(r2, r2, 16);
            rtl_or(r1, r1, r2);
            rtl_msb(r2, r1, 4);
            rtl_sub(r2, rzero, r2);
            break;
        case 4:
            rtl_lr_l(r1, R_EAX);
            rtl_lr_l(r2, R_EDX);
            break;
        default: assert(0);
    }

    rtl_idiv(r1, r0, r2, r1, r0);

    rtl_sr(R_EAX, id_dest->width, r1);
    if (id_dest->width == 1)
        rtl_sr_b(R_AH, r0);
    else
        rtl_sr(R_EDX, id_dest->width, r0);

    print_asm_template1(idiv);
}