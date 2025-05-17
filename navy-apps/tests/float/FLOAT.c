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

FLOAT F_mul_F(FLOAT a, FLOAT b) { return (FLOAT)(((int64_t)a * b) >> 16); }

FLOAT F_div_F(FLOAT a, FLOAT b)
{
    assert(b != 0);
    return (FLOAT)(((int64_t)a << 16) / b);
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

    float_u_caster caster;
    caster.f      = a;
    uint32_t bits = caster.u;

    uint32_t sign_bit       = (bits >> 31);
    int32_t  exp_biased     = (bits >> 23) & 0xFF;
    uint32_t mant_frac_bits = bits & 0x007FFFFF;

    if (exp_biased == 0xFF) {
        if (mant_frac_bits != 0)
            return 0;
        else
            return sign_bit ? (FLOAT)0x80000000 : (FLOAT)0x7FFFFFFF;
    }

    if (exp_biased == 0 && mant_frac_bits == 0) return 0;

    int32_t  exponent_unbiased = exp_biased - 127;
    uint32_t abs_mantissa;

    if (exp_biased == 0) {
        abs_mantissa = mant_frac_bits;
        exponent_unbiased       = -126;
    }
    else
        abs_mantissa = mant_frac_bits | 0x00800000;

    int     shift_amount = exponent_unbiased - 7;
    int64_t abs_result_64;

    if (abs_mantissa == 0)
        abs_result_64 = 0;
    else if (shift_amount >= (63 - 24))
        abs_result_64 = 0x7FFFFFFFFFFFFFFFLL;
    else if (shift_amount < -(63))
        abs_result_64 = 0;
    else
    {
        if (shift_amount >= 0)
            abs_result_64 = (int64_t)abs_mantissa << shift_amount;
        else
            abs_result_64 = (int64_t)abs_mantissa >> (-shift_amount);
    }

    FLOAT final_result;
    if (sign_bit) {
        if (abs_result_64 >= 0x80000000LL)
            final_result = (FLOAT)0x80000000;
        else
            final_result = (FLOAT)(-abs_result_64);
    }
    else
    {
        if (abs_result_64 >= 0x7FFFFFFFLL)
            final_result = (FLOAT)0x7FFFFFFF;
        else
            final_result = (FLOAT)abs_result_64;
    }
    return final_result;
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
