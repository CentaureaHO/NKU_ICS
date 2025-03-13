#ifndef __REG_H__
#define __REG_H__

#include "common.h"

#ifndef X86_REGS
#define X86_REGS  \
    X(eax, 32)    \
    X(ecx, 32)    \
    X(edx, 32)    \
    X(ebx, 32)    \
    X(esp, 32)    \
    X(ebp, 32)    \
    X(esi, 32)    \
    X(edi, 32)    \
    X(eip, 32)    \
    X(ax, 16)     \
    X(cx, 16)     \
    X(dx, 16)     \
    X(bx, 16)     \
    X(sp, 16)     \
    X(bp, 16)     \
    X(si, 16)     \
    X(di, 16)     \
    X(al, 8)      \
    X(cl, 8)      \
    X(dl, 8)      \
    X(bl, 8)      \
    X(ah, 8)      \
    X(ch, 8)      \
    X(dh, 8)      \
    X(bh, 8)      \
    X(eflags, 32) \
    X(CF, 1)      \
    X(PF, 1)      \
    X(AF, 1)      \
    X(ZF, 1)      \
    X(SF, 1)      \
    X(TF, 1)      \
    X(IF, 1)      \
    X(DF, 1)      \
    X(OF, 1)      \
    X(IOPL, 2)    \
    X(NT, 1)      \
    X(RF, 1)      \
    X(VM, 1)
#endif

#define X86_FLAGS \
    X(CF, 0)      \
    X(PF, 2)      \
    X(AF, 4)      \
    X(ZF, 6)      \
    X(SF, 7)      \
    X(TF, 8)      \
    X(IF, 9)      \
    X(DF, 10)     \
    X(OF, 11)     \
    X(IOPL_L, 12) \
    X(IOPL_H, 13) \
    X(NT, 14)     \
    X(RF, 16)     \
    X(VM, 17)

enum
{
    R_EAX,
    R_ECX,
    R_EDX,
    R_EBX,
    R_ESP,
    R_EBP,
    R_ESI,
    R_EDI
};
enum
{
    R_AX,
    R_CX,
    R_DX,
    R_BX,
    R_SP,
    R_BP,
    R_SI,
    R_DI
};
enum
{
    R_AL,
    R_CL,
    R_DL,
    R_BL,
    R_AH,
    R_CH,
    R_DH,
    R_BH
};

/* DONE: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct
{
    union
    {
        struct
        {
#define CPU_STATE_REG_DECL(n)              \
    union                                  \
    {                                      \
        uint32_t e##n##x; /* 32-bit */     \
        uint16_t n##x;    /* 16-bit */     \
        struct                             \
        {                                  \
            uint8_t n##l; /* low 8-bit */  \
            uint8_t n##h; /* high 8-bit */ \
        };                                 \
    }
            CPU_STATE_REG_DECL(a);  // index 0: eax/ax/al/ah
            CPU_STATE_REG_DECL(c);  // index 1: ecx/cx/cl/ch
            CPU_STATE_REG_DECL(d);  // index 2: edx/dx/dl/dh
            CPU_STATE_REG_DECL(b);  // index 3: ebx/bx/bl/bh
#undef CPU_STATE_REG_DECL
#define CPU_STATE_REG_DECL(n)       \
    union                           \
    {                               \
        uint32_t e##n; /* 32-bit */ \
        uint16_t n;    /* 16-bit */ \
    }
            CPU_STATE_REG_DECL(sp);  // index 4: esp/sp
            CPU_STATE_REG_DECL(bp);  // index 5: ebp/bp
            CPU_STATE_REG_DECL(si);  // index 6: esi/si
            CPU_STATE_REG_DECL(di);  // index 7: edi/di
#undef CPU_STATE_REG_DECL
        };

        struct
        {
            union
            {
                uint32_t _32;
                uint16_t _16;
                uint8_t  _8[2];
            } gpr[8];
        };
    };

    vaddr_t eip;

    union
    {
        uint32_t eflags;
        struct
        {
            uint32_t CF : 1;
            const uint32_t : 1;
            uint32_t PF : 1;
            const uint32_t : 1;
            uint32_t AF : 1;
            const uint32_t : 1;
            uint32_t ZF : 1;
            uint32_t SF : 1;
            uint32_t TF : 1;
            uint32_t IF : 1;
            uint32_t DF : 1;
            uint32_t OF : 1;
            uint32_t IOPL : 2;
            uint32_t NT : 1;
            const uint32_t : 1;
            uint32_t RF : 1;
            uint32_t VM : 1;
            const uint32_t : 14;
        };
    };

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index)
{
    assert(index >= 0 && index < 8);
    return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

static inline const char* reg_name(int index, int width)
{
    assert(index >= 0 && index < 8);
    switch (width)
    {
        case 4: return regsl[index];
        case 1: return regsb[index];
        case 2: return regsw[index];
        default: assert(0);
    }
}

#endif
