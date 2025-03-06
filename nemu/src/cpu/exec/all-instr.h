#include "cpu/exec.h"

make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(call);

// group 1
// #define MAKE_ARITH_EHELPER(name) make_EHelper(concat3(arith, _, name))
#define MAKE_ARITH_EHELPER(name) make_EHelper(name)

MAKE_ARITH_EHELPER(add);
MAKE_ARITH_EHELPER(or);
MAKE_ARITH_EHELPER(adc);
MAKE_ARITH_EHELPER(sbb);
MAKE_ARITH_EHELPER(and);
MAKE_ARITH_EHELPER(sub);
MAKE_ARITH_EHELPER(xor);
MAKE_ARITH_EHELPER(cmp);

#undef MAKE_ARITH_EHELPER
