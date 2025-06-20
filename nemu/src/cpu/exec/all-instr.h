#include "cpu/exec.h"

// arith
make_EHelper(add);
make_EHelper(adc);
make_EHelper(sbb);
make_EHelper(sub);
make_EHelper(cmp);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(idiv);
make_EHelper(div);
make_EHelper(neg);
make_EHelper(mul);

// data-mov
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(cltd);
make_EHelper(movsx);
make_EHelper(leave);
make_EHelper(cwtl);
make_EHelper(pusha);
make_EHelper(popa);
make_EHelper(movsb);

make_EHelper(operand_size);

// special
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);

// control
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);

// logic
make_EHelper(test);
make_EHelper(and);
make_EHelper (xor);
make_EHelper(setcc);
make_EHelper(or);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(shr);
make_EHelper(not);
make_EHelper(rol);
make_EHelper(ror);

// system
make_EHelper(in);
make_EHelper(out);
make_EHelper(int);
make_EHelper(into);
make_EHelper(iret);
make_EHelper(lidt);
