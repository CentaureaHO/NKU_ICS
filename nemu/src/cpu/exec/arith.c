#include "cpu/exec.h"

make_EHelper(add)
{
    // OF, SF, ZF, AF, CF, and PF as described in Appendix C
    TODO();

    print_asm_template2(add);
}

make_EHelper(sub)
{
    // OF, SF, ZF, AF, PF, and CF as described in Appendix C
    TODO();

    print_asm_template2(sub);
}

make_EHelper(cmp)
{
    // OF, SF, ZF, AF, PF, and CF as described in Appendix C
    TODO();

    print_asm_template2(cmp);
}

make_EHelper(inc)
{
    // OF, SF, ZF, AF, and PF as described in Appendix C
    TODO();

    print_asm_template1(inc);
}

make_EHelper(dec)
{
    // OF, SF, ZF, AF, and PF as described in Appendix C.
    TODO();

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
    TODO();

    print_asm_template2(adc);
}

make_EHelper(sbb)
{
    // OF, SF, ZF, AF, PF, and CF as described in Appendix C
    TODO();

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
    TODO();

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
    // OF and CF as described above; SF, ZF, AF, PF, and CF are undefined
    TODO();

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
    TODO();

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
    TODO();

    print_asm_template3(imul);
}

make_EHelper(div)
{
    // OF, SF, ZF, AR(AF), PF, CF are undefined.
    TODO();

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