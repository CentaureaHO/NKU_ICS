#ifndef __RTL_H__
#define __RTL_H__

#include "nemu.h"

#define TRUNCATE_MASK(width) (~0u >> ((4 - width) << 3))
#define SIGNED_BIT_MASK(width) (1u << ((width << 3) - 1))
#define RANGE_MASK(high, low) ((~0u >> (31 - (high))) & (~0u << (low)))

extern rtlreg_t        t0, t1, t2, t3;
extern rtlreg_t*       r0;
extern rtlreg_t*       r1;
extern rtlreg_t*       r2;
extern rtlreg_t*       r3;
extern const rtlreg_t  tzero;
extern const rtlreg_t* rzero;

extern const rtlreg_t  ENABLE, DISABLE;
extern const rtlreg_t* enable;
extern const rtlreg_t* disable;

/* RTL basic instructions */

static inline void rtl_li(rtlreg_t* dest, uint32_t imm) { *dest = imm; }

#define c_add(a, b) ((a) + (b))
#define c_sub(a, b) ((a) - (b))
#define c_and(a, b) ((a) & (b))
#define c_or(a, b) ((a) | (b))
#define c_xor(a, b) ((a) ^ (b))
#define c_shl(a, b) ((a) << (b))
#define c_shr(a, b) ((a) >> (b))
#define c_sar(a, b) ((int32_t)(a) >> (b))
#define c_slt(a, b) ((int32_t)(a) < (int32_t)(b))
#define c_sltu(a, b) ((a) < (b))

#define make_rtl_arith_logic(name)                                                                     \
    static inline void concat(rtl_, name)(rtlreg_t * dest, const rtlreg_t* src1, const rtlreg_t* src2) \
    {                                                                                                  \
        *dest = concat(c_, name)(*src1, *src2);                                                        \
    }                                                                                                  \
    static inline void concat3(rtl_, name, i)(rtlreg_t * dest, const rtlreg_t* src1, int imm)          \
    {                                                                                                  \
        *dest = concat(c_, name)(*src1, imm);                                                          \
    }

make_rtl_arith_logic(add) make_rtl_arith_logic(sub) make_rtl_arith_logic(and) make_rtl_arith_logic(or)
    make_rtl_arith_logic (xor) make_rtl_arith_logic(shl) make_rtl_arith_logic(shr) make_rtl_arith_logic(sar)
        make_rtl_arith_logic(slt) make_rtl_arith_logic(sltu)

            static inline void rtl_mul(rtlreg_t* dest_hi, rtlreg_t* dest_lo, const rtlreg_t* src1, const rtlreg_t* src2)
{
    asm volatile("mul %3" : "=d"(*dest_hi), "=a"(*dest_lo) : "a"(*src1), "r"(*src2));
}

static inline void rtl_imul(rtlreg_t* dest_hi, rtlreg_t* dest_lo, const rtlreg_t* src1, const rtlreg_t* src2)
{
    asm volatile("imul %3" : "=d"(*dest_hi), "=a"(*dest_lo) : "a"(*src1), "r"(*src2));
}

static inline void rtl_div(
    rtlreg_t* q, rtlreg_t* r, const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
{
    asm volatile("div %4" : "=a"(*q), "=d"(*r) : "d"(*src1_hi), "a"(*src1_lo), "r"(*src2));
}

static inline void rtl_idiv(
    rtlreg_t* q, rtlreg_t* r, const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
{
    asm volatile("idiv %4" : "=a"(*q), "=d"(*r) : "d"(*src1_hi), "a"(*src1_lo), "r"(*src2));
}

static inline void rtl_lm(rtlreg_t* dest, const rtlreg_t* addr, int len) { *dest = vaddr_read(*addr, len); }

static inline void rtl_sm(rtlreg_t* addr, int len, const rtlreg_t* src1) { vaddr_write(*addr, len, *src1); }

static inline void rtl_lr_b(rtlreg_t* dest, int r) { *dest = reg_b(r); }

static inline void rtl_lr_w(rtlreg_t* dest, int r) { *dest = reg_w(r); }

static inline void rtl_lr_l(rtlreg_t* dest, int r) { *dest = reg_l(r); }

static inline void rtl_sr_b(int r, const rtlreg_t* src1) { reg_b(r) = *src1; }

static inline void rtl_sr_w(int r, const rtlreg_t* src1) { reg_w(r) = *src1; }

static inline void rtl_sr_l(int r, const rtlreg_t* src1) { reg_l(r) = *src1; }

static inline void rtl_sr_idtr_b(const rtlreg_t* src1) { cpu.idtr.base = *src1; }

static inline void rtl_sr_idtr_w(const rtlreg_t* src1) { cpu.idtr.limit = *src1; }

/* RTL psuedo instructions */

static inline void rtl_lr(rtlreg_t* dest, int r, int width)
{
    switch (width)
    {
        case 4: rtl_lr_l(dest, r); return;
        case 1: rtl_lr_b(dest, r); return;
        case 2: rtl_lr_w(dest, r); return;
        default: assert(0);
    }
}

static inline void rtl_sr(int r, int width, const rtlreg_t* src1)
{
    switch (width)
    {
        case 4: rtl_sr_l(r, src1); return;
        case 1: rtl_sr_b(r, src1); return;
        case 2: rtl_sr_w(r, src1); return;
        default: assert(0);
    }
}

#define make_rtl_setget_eflags(f)                                                 \
    static inline void concat(rtl_set_, f)(const rtlreg_t* src) { cpu.f = *src; } \
    static inline void concat(rtl_get_, f)(rtlreg_t * dest) { *dest = cpu.f; }

make_rtl_setget_eflags(CF) make_rtl_setget_eflags(OF) make_rtl_setget_eflags(ZF) make_rtl_setget_eflags(SF)
    make_rtl_setget_eflags(PF) make_rtl_setget_eflags(AF)

        static inline void rtl_mv(rtlreg_t* dest, const rtlreg_t* src1)
{
    // dest <- src1
    // TODO();
    *dest = *src1;
}

static inline void rtl_not(rtlreg_t* dest)
{
    // dest <- ~dest
    // TODO();
    *dest = ~*dest;
}

static inline void rtl_sext(rtlreg_t* dest, const rtlreg_t* src1, int width)
{
    if (width == 4) {
        *dest = *src1;
        return;
    }

    switch (width)
    {
        case 1: *dest = (int8_t)(*src1 & 0xFF); break;
        case 2: *dest = (int16_t)(*src1 & 0xFFFF); break;
        default: Assert(0, "Invalid width");
    }
}

static inline void rtl_push(const rtlreg_t* src1)
{
    // esp <- esp - 4
    // M[esp] <- src1
    cpu.esp -= 4;
    vaddr_write(cpu.esp, 4, *src1);
}

static inline void rtl_pop(rtlreg_t* dest)
{
    // dest <- M[esp]
    // esp <- esp + 4
    *dest = vaddr_read(cpu.esp, 4);
    cpu.esp += 4;
}

static inline void rtl_rol(rtlreg_t* dest, const rtlreg_t* src1, const rtlreg_t* src2, int width)
{
    // dest <- { src1[width * 8 - 1 - src2: 0], src1[width * 8 - 1: src2] }
    // CF <- dest[0]
    // Assert(*src2 < width << 3, "Invalid src2 = %d for width = %d", *src2, width);

    int total_bits = width << 3;
    int shift      = *src2 % total_bits;

    if (shift == 0)
        *dest = *src1;
    else
    {
        uint32_t mask = TRUNCATE_MASK(width);
        *dest         = ((*src1 << shift) | (*src1 >> (total_bits - shift))) & mask;
    }
}

static inline void rtl_ror(rtlreg_t* dest, const rtlreg_t* src1, const rtlreg_t* src2, int width)
{
    // dest <- { src1[src2 - 1: 0], src1[width * 8 - 1: src2] }
    // CF <- dest[width * 8 - 1]
    // Assert(*src2 < width << 3, "Invalid src2 = %d for width = %d", *src2, width);

    int total_bits = width << 3;
    int shift      = *src2 % total_bits;

    if (shift == 0)
        *dest = *src1;
    else
    {
        uint32_t mask = TRUNCATE_MASK(width);
        *dest         = ((*src1 >> shift) | (*src1 << (total_bits - shift))) & mask;
    }
}

static inline void rtl_eq0(rtlreg_t* dest, const rtlreg_t* src1)
{
    // dest <- (src1 == 0 ? 1 : 0)
    *dest = (*src1 == 0) ? 1 : 0;
}

static inline void rtl_eqi(rtlreg_t* dest, const rtlreg_t* src1, int imm)
{
    // dest <- (src1 == imm ? 1 : 0)
    *dest = (*src1 == imm) ? 1 : 0;
}

static inline void rtl_neq0(rtlreg_t* dest, const rtlreg_t* src1)
{
    // dest <- (src1 != 0 ? 1 : 0)
    *dest = (*src1 != 0) ? 1 : 0;
}

void rtl_msb(rtlreg_t* dest, const rtlreg_t* src1, int width)
{
    // dest <- src1[width * 8 - 1]
    *dest = (*src1 & SIGNED_BIT_MASK(width)) ? 1 : 0;
}

static inline void rtl_mtb(rtlreg_t* dest, const rtlreg_t* src1, int tar)
{
    // dest <- src1[tar]
    *dest = (*src1 >> tar) & 1;
}

static inline void rtl_update_ZF(const rtlreg_t* result, int width)
{
    // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
    // TODO();
    if ((*result & TRUNCATE_MASK(width)) == 0)
        rtl_set_ZF(enable);
    else
        rtl_set_ZF(disable);
}

static inline void rtl_update_SF(const rtlreg_t* result, int width)
{
    // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
    // TODO();
    if (*result & SIGNED_BIT_MASK(width))
        rtl_set_SF(enable);
    else
        rtl_set_SF(disable);
}

static inline void rtl_update_ZFSF(const rtlreg_t* result, int width)
{
    rtl_update_ZF(result, width);
    rtl_update_SF(result, width);
}

static inline void rtl_update_PF(const rtlreg_t* result)
{
    uint8_t src = *result & TRUNCATE_MASK(1);
    int     cnt = 0;
    while (src) {
        src &= src - 1;
        ++cnt;
    }

    if (cnt % 2 == 0)
        rtl_set_PF(enable);
    else
        rtl_set_PF(disable);
}

static inline void rtl_update_PFZFSF(const rtlreg_t* result, int width)
{
    rtl_update_PF(result);
    rtl_update_ZFSF(result, width);
}

#endif
