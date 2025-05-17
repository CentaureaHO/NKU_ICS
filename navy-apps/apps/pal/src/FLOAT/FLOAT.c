#include "FLOAT.h"
#include <assert.h>
#include <stdint.h>

FLOAT F_mul_F(FLOAT a, FLOAT b)
{
    int32_t val_a = a;
    int32_t val_b = b;
    int64_t product;
    FLOAT   result;

    asm volatile("imul %%edx\n\t"
                 "shrd %%cl, %%edx, %%eax\n\t"
                 "movl %%eax, %0\n\t"
                 : "=r"(result)
                 : "a"(val_a), "d"(val_b), "c"(16)
                 : "cc");
    return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b)
{
    assert(b != 0);
    int32_t val_a = a;
    int32_t val_b = b;
    int32_t quotient_val;

    asm volatile("movl %1, %%eax\n\t"   // eax = val_a
                 "movl %1, %%edx\n\t"   // edx = val_a (for sign extension)
                 "sall $16, %%eax\n\t"  // eax = val_a << 16 (lower part of dividend)
                 "sarl $16, %%edx\n\t"  // edx = val_a >> 16 (upper part of dividend, sign extended)
                                        // EDX:EAX = (int64_t)val_a << 16
                 "idivl %2\n\t"         // eax = EDX:EAX / val_b (quotient), edx = remainder
                 "movl %%eax, %0\n\t"
                 : "=r"(quotient_val)
                 : "r"(val_a), "r"(val_b)
                 : "%eax", "%edx", "cc");
    return quotient_val;
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
        abs_mantissa      = mant_frac_bits;
        exponent_unbiased = -126;
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
        {
            int right_shift = -shift_amount;
            if (right_shift >= 64)
                abs_result_64 = 0;
            else
                abs_result_64 = (int64_t)abs_mantissa >> right_shift;
        }
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
