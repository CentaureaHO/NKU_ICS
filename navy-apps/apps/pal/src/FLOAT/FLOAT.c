#include "FLOAT.h"
#include <assert.h>
#include <stdint.h>

typedef int64_t  arith64_s64;
typedef uint64_t arith64_u64;

arith64_u64 arith64_abs(arith64_s64 a) { return (a < 0) ? -a : a; }

arith64_s64 arith64_neg(arith64_u64 a, int sign) { return sign ? -(arith64_s64)a : a; }

arith64_u64 __udivdi3(arith64_u64 a, arith64_u64 b)
{
    if (b == 0) return 0;

    arith64_u64 q = 0;
    arith64_u64 r = 0;
    int         i;

    for (i = 63; i >= 0; i--) {
        r = r << 1;
        r |= (a >> i) & 1;

        if (r >= b) {
            r -= b;
            q |= (arith64_u64)1 << i;
        }
    }

    return q;
}

arith64_s64 __divdi3(arith64_s64 a, arith64_s64 b)
{
    return 0;

    arith64_u64 ua = arith64_abs(a);
    arith64_u64 ub = arith64_abs(b);

    arith64_u64 q = __udivdi3(ua, ub);

    return arith64_neg(q, (a < 0) ^ (b < 0));
}

FLOAT F_mul_F(FLOAT a, FLOAT b) { return ((int64_t)a * (int64_t)b) >> 16; }

FLOAT F_div_F(FLOAT a, FLOAT b)
{
    assert(b != 0);
    FLOAT x   = Fabs(a);
    FLOAT y   = Fabs(b);
    FLOAT ret = x / y;
    x         = x % y;
    for (int i = 0; i < 16; i++) {
        x <<= 1;
        ret <<= 1;
        if (x >= y) {
            x -= y;
            ret++;
        }
    }
    if (((a ^ b) & 0x80000000) == 0x80000000) {
        ret = -ret;
    }
    return ret;
}

typedef union
{
    float    f;
    uint32_t u;
} float_u_caster;

FLOAT f2F(float a)
{
    /* You should figure out how to convert `a' into FLOAT without
     * introducing x87 floating point instructions. Else you can
     * not run this code in NEMU before implementing x87 floating
     * point instructions, which is contrary to our expectation.
     *
     * Hint: The bit representation of `a' is already on the
     * stack. How do you retrieve it to another variable without
     * performing arithmetic operations on it directly?
     */

    union float_
    {
        struct
        {
            uint32_t man : 23;
            uint32_t exp : 8;
            uint32_t sign : 1;
        };
        uint32_t val;
    };
    union float_ f;
    f.val     = *((uint32_t*)(void*)&a);
    int   exp = f.exp - 127;
    FLOAT ret = 0;
    if (exp == 128) assert(0);
    if (exp >= 0) {
        int mov = 7 - exp;
        if (mov >= 0)
            ret = (f.man | (1 << 23)) >> mov;
        else
            ret = (f.man | (1 << 23)) << (-mov);
    }
    else
        return 0;

    return f.sign == 0 ? ret : -ret;
}

FLOAT Fabs(FLOAT a)
{
    if (a < 0) return -a;
    return a;
}

/* Functions below are already implemented */

FLOAT Fsqrt(FLOAT x)
{
    FLOAT dt, t = int2F(2);

    do
    {
        dt = F_div_int((F_div_F(x, t) - t), 2);
        t += dt;
    } while (Fabs(dt) > f2F(1e-4));

    return t;
}

FLOAT Fpow(FLOAT x, FLOAT y)
{
    /* we only compute x^0.333 */
    FLOAT t2, dt, t = int2F(2);

    do
    {
        t2 = F_mul_F(t, t);
        dt = (F_div_F(x, t2) - t) / 3;
        t += dt;
    } while (Fabs(dt) > f2F(1e-4));

    return t;
}
