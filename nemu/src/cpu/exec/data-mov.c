#include "cpu/exec.h"

make_EHelper(mov)
{
    // Flags Affected: None

    rtl_li(r0, id_src->val);

    operand_write(id_dest, r0);
    print_asm_template2(mov);
}

make_EHelper(push)
{
    // Flags Affected: None

    rtl_li(r0, id_dest->val);
    // rtl_push(r0);
    rtl_sext(&t0, r0, id_dest->width);
    rtl_push(&t0);

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
    rtl_lr(r0, R_ESP, 4);

    if (decoding.is_operand_size_16) {
        rtl_lr(r1, R_AX, 2);
        rtl_push(r1);
        rtl_lr(r1, R_CX, 2);
        rtl_push(r1);
        rtl_lr(r1, R_DX, 2);
        rtl_push(r1);
        rtl_lr(r1, R_BX, 2);
        rtl_push(r1);
        rtl_push(r0);
        rtl_lr(r1, R_BP, 2);
        rtl_push(r1);
        rtl_lr(r1, R_SI, 2);
        rtl_push(r1);
        rtl_lr(r1, R_DI, 2);
        rtl_push(r1);
    }
    else
    {
        rtl_lr(r1, R_EAX, 4);
        rtl_push(r1);
        rtl_lr(r1, R_ECX, 4);
        rtl_push(r1);
        rtl_lr(r1, R_EDX, 4);
        rtl_push(r1);
        rtl_lr(r1, R_EBX, 4);
        rtl_push(r1);
        rtl_push(r0);
        rtl_lr(r1, R_EBP, 4);
        rtl_push(r1);
        rtl_lr(r1, R_ESI, 4);
        rtl_push(r1);
        rtl_lr(r1, R_EDI, 4);
        rtl_push(r1);
    }

    print_asm("pusha");
}

make_EHelper(popa)
{
    if (decoding.is_operand_size_16)
        rtl_li(r0, 2);
    else
        rtl_li(r0, 4);

    rtl_pop(r1);
    rtl_sr(R_EDI, t0, r1);
    rtl_pop(r1);
    rtl_sr(R_ESI, t0, r1);
    rtl_pop(r1);
    rtl_sr(R_EBP, t0, r1);
    rtl_pop(r1);
    rtl_pop(r1);
    rtl_sr(R_EBX, t0, r1);
    rtl_pop(r1);
    rtl_sr(R_EDX, t0, r1);
    rtl_pop(r1);
    rtl_sr(R_ECX, t0, r1);
    rtl_pop(r1);
    rtl_sr(R_EAX, t0, r1);

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

make_EHelper(movsb)
{
  rtl_lr(&t0, R_ESI, 4);
  rtl_lm(&t2, &t0, 1);

  rtl_lr(&t1, R_EDI, 4);
  rtl_sm(&t1, 1, &t2);

  rtl_lr(&t0, R_ESI, 4);
  rtl_lr(&t1, R_EDI, 4);
  
  rtl_get_DF(&t3);
  if (t3 == 0) 
  {
    rtl_addi(&t0, &t0, 1);
    rtl_addi(&t1, &t1, 1);
  } 
  else 
  {
    rtl_subi(&t0, &t0, 1);
    rtl_subi(&t1, &t1, 1);
  }
  
  rtl_sr(R_ESI, 4, &t0);
  rtl_sr(R_EDI, 4, &t1);

  print_asm("movsb");
}

make_EHelper(lea)
{
    // Flags Affected: None

    rtl_li(&t2, id_src->addr);
    operand_write(id_dest, &t2);
    print_asm_template2(lea);
}