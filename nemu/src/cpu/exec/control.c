#include "cpu/exec.h"

make_EHelper(jmp)
{
<<<<<<< HEAD
    // the target address is calculated at the decode stage
    decoding.is_jmp = 1;

=======
    // Flags Affected: All if a task switch takes place; none if no task switch occurs

    decoding.is_jmp = 1;

>>>>>>> master
    print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc)
{
<<<<<<< HEAD
    // the target address is calculated at the decode stage
    uint8_t subcode = decoding.opcode & 0xf;
    rtl_setcc(&t2, subcode);
    decoding.is_jmp = t2;

=======
    // Flags Affected: None

    uint8_t subcode = decoding.opcode & 0xf;
    rtl_setcc(r0, subcode);
    decoding.is_jmp = t0;

>>>>>>> master
    print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm)
{
    decoding.jmp_eip = id_dest->val;
    decoding.is_jmp  = 1;

    print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call)
{
<<<<<<< HEAD
    // the target address is calculated at the decode stage
    TODO();

=======
    // Flags Affected: All if a task switch takes place; none if no task switch occurs

    decoding.is_jmp = 1;
    rtl_push(&decoding.seq_eip);

>>>>>>> master
    print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret)
{
<<<<<<< HEAD
    TODO();

=======
    // Flags Affected: None

    rtl_pop(r0);
    decoding.jmp_eip = t0;
    decoding.is_jmp  = 1;

>>>>>>> master
    print_asm("ret");
}

make_EHelper(call_rm)
{
    TODO();

    print_asm("call *%s", id_dest->str);
<<<<<<< HEAD
}
=======
}
>>>>>>> master
