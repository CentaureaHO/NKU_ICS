#include "cpu/exec.h"

// data-mov
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);

make_EHelper(operand_size);

// special
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);

// control
make_EHelper(call);
make_EHelper(ret);

// logic
make_EHelper(setcc);

// group 1
make_EHelper(add);
make_EHelper(or);
make_EHelper(adc);
make_EHelper(sbb);
make_EHelper(and);
make_EHelper(sub);
make_EHelper (xor);
make_EHelper(cmp);

// group 5
make_EHelper(inc);
make_EHelper(dec);
// make_EHelper(call); declared at control
// make_EHelper(call); declared at control
make_EHelper(jmp);
// make_EHelper(jmp); declared at group 5
// make_EHelper(push); declared at data-mov
// Empty