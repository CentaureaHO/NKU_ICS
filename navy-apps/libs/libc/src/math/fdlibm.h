
/* @(#)fdlibm.h 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/* CYGNUS LOCAL: Include files.  */
#include <machine/ieeefp.h>
#include <math.h>

// @jyy: just make it compile
#define _LIB_VERSION 1
#define _IEEE_ 1
#define _POSIX_ 2
#define _SVID_ 3

/* CYGNUS LOCAL: reentrancy stuff.  */
#include <reent.h>

#ifdef _LIBM_REENT
#define _R1 struct _reent *reent_ptr,
#define _R2 reent_ptr,
#define _R3 struct reent_ptr;
#define _R4 reent_ptr
#else
#define _R1
#define _R2
#define _R3
#define _R4 _REENT
#endif

/* CYGNUS LOCAL: Default to XOPEN_MODE.  */
#define _XOPEN_MODE

#ifdef __STDC__
#define __P(p) p
#else
#define __P(p) ()
#endif

#define HUGE ((float)3.40282346638528860e+38)

/*
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */

#define X_TLOSS 1.41484755040568800000e+16

/* Functions that are not documented, and are not in <math.h>.  */

extern double logb __P((double));
#ifdef _SCALB_INT
extern double scalb __P((double, int));
#else
extern double scalb __P((double, double));
#endif
extern double significand __P((double));

/* ieee style elementary functions */
extern double __ieee754_sqrt __P((double));
extern double __ieee754_acos __P((double));
extern double __ieee754_acosh __P((double));
extern double __ieee754_log __P((double));
extern double __ieee754_atanh __P((double));
extern double __ieee754_asin __P((double));
extern double __ieee754_atan2 __P((double, double));
extern double __ieee754_exp __P((double));
extern double __ieee754_cosh __P((double));
extern double __ieee754_fmod __P((double, double));
extern double __ieee754_pow __P((double, double));
<<<<<<< HEAD
extern double __ieee754_lgamma_r __P((double, int*));
extern double __ieee754_gamma_r __P((double, int*));
=======
extern double __ieee754_lgamma_r __P((double, int *));
extern double __ieee754_gamma_r __P((double, int *));
>>>>>>> master
extern double __ieee754_log10 __P((double));
extern double __ieee754_sinh __P((double));
extern double __ieee754_hypot __P((double, double));
extern double __ieee754_j0 __P((double));
extern double __ieee754_j1 __P((double));
extern double __ieee754_y0 __P((double));
extern double __ieee754_y1 __P((double));
extern double __ieee754_jn __P((int, double));
extern double __ieee754_yn __P((int, double));
extern double __ieee754_remainder __P((double, double));
<<<<<<< HEAD
extern __int32_t __ieee754_rem_pio2 __P((double, double*));
=======
extern __int32_t __ieee754_rem_pio2 __P((double, double *));
>>>>>>> master
#ifdef _SCALB_INT
extern double __ieee754_scalb __P((double, int));
#else
extern double __ieee754_scalb __P((double, double));
#endif

/* fdlibm kernel function */
<<<<<<< HEAD
extern double __kernel_standard __P((struct _reent*, double, double, int));
extern double __kernel_sin __P((double, double, int));
extern double __kernel_cos __P((double, double));
extern double __kernel_tan __P((double, double, int));
extern int __kernel_rem_pio2 __P((double*, double*, int, int, int, const __int32_t*));
=======
extern double __kernel_standard __P((struct _reent *, double, double, int));
extern double __kernel_sin __P((double, double, int));
extern double __kernel_cos __P((double, double));
extern double __kernel_tan __P((double, double, int));
extern int __kernel_rem_pio2 __P((double *, double *, int, int, int,
                                  const __int32_t *));
>>>>>>> master

/* Undocumented float functions.  */
extern float logbf __P((float));
#ifdef _SCALB_INT
extern float scalbf __P((float, int));
#else
extern float scalbf __P((float, float));
#endif
extern float significandf __P((float));

/* ieee style elementary float functions */
extern float __ieee754_sqrtf __P((float));
extern float __ieee754_acosf __P((float));
extern float __ieee754_acoshf __P((float));
extern float __ieee754_logf __P((float));
extern float __ieee754_atanhf __P((float));
extern float __ieee754_asinf __P((float));
extern float __ieee754_atan2f __P((float, float));
extern float __ieee754_expf __P((float));
extern float __ieee754_coshf __P((float));
extern float __ieee754_fmodf __P((float, float));
extern float __ieee754_powf __P((float, float));
<<<<<<< HEAD
extern float __ieee754_lgammaf_r __P((float, int*));
extern float __ieee754_gammaf_r __P((float, int*));
=======
extern float __ieee754_lgammaf_r __P((float, int *));
extern float __ieee754_gammaf_r __P((float, int *));
>>>>>>> master
extern float __ieee754_log10f __P((float));
extern float __ieee754_sinhf __P((float));
extern float __ieee754_hypotf __P((float, float));
extern float __ieee754_j0f __P((float));
extern float __ieee754_j1f __P((float));
extern float __ieee754_y0f __P((float));
extern float __ieee754_y1f __P((float));
extern float __ieee754_jnf __P((int, float));
extern float __ieee754_ynf __P((int, float));
extern float __ieee754_remainderf __P((float, float));
<<<<<<< HEAD
extern __int32_t __ieee754_rem_pio2f __P((float, float*));
=======
extern __int32_t __ieee754_rem_pio2f __P((float, float *));
>>>>>>> master
#ifdef _SCALB_INT
extern float __ieee754_scalbf __P((float, int));
#else
extern float __ieee754_scalbf __P((float, float));
#endif

/* float versions of fdlibm kernel functions */
extern float __kernel_sinf __P((float, float, int));
extern float __kernel_cosf __P((float, float));
extern float __kernel_tanf __P((float, float, int));
<<<<<<< HEAD
extern int __kernel_rem_pio2f __P((float*, float*, int, int, int, const __int32_t*));
=======
extern int __kernel_rem_pio2f __P((float *, float *, int, int, int,
                                   const __int32_t *));
>>>>>>> master

/* The original code used statements like
        n0 = ((*(int*)&one)>>29)^1;		* index of high word *
        ix0 = *(n0+(int*)&x);			* high word of x *
        ix1 = *((1-n0)+(int*)&x);		* low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

#ifndef __IEEE_BIG_ENDIAN
#ifndef __IEEE_LITTLE_ENDIAN
#error Must define endianness
#endif
#endif

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

#ifdef __IEEE_BIG_ENDIAN

<<<<<<< HEAD
typedef union
{
    double value;
    struct
    {
        __uint32_t msw;
        __uint32_t lsw;
    } parts;
=======
typedef union {
  double value;
  struct {
    __uint32_t msw;
    __uint32_t lsw;
  } parts;
>>>>>>> master
} ieee_double_shape_type;

#endif

#ifdef __IEEE_LITTLE_ENDIAN

<<<<<<< HEAD
typedef union
{
    double value;
    struct
    {
        __uint32_t lsw;
        __uint32_t msw;
    } parts;
=======
typedef union {
  double value;
  struct {
    __uint32_t lsw;
    __uint32_t msw;
  } parts;
>>>>>>> master
} ieee_double_shape_type;

#endif

/* Get two 32 bit ints from a double.  */

<<<<<<< HEAD
#define EXTRACT_WORDS(ix0, ix1, d)   \
    do                               \
    {                                \
        ieee_double_shape_type ew_u; \
        ew_u.value = (d);            \
        (ix0)      = ew_u.parts.msw; \
        (ix1)      = ew_u.parts.lsw; \
    } while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i, d)          \
    do                               \
    {                                \
        ieee_double_shape_type gh_u; \
        gh_u.value = (d);            \
        (i)        = gh_u.parts.msw; \
    } while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i, d)           \
    do                               \
    {                                \
        ieee_double_shape_type gl_u; \
        gl_u.value = (d);            \
        (i)        = gl_u.parts.lsw; \
    } while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d, ix0, ix1)    \
    do                               \
    {                                \
        ieee_double_shape_type iw_u; \
        iw_u.parts.msw = (ix0);      \
        iw_u.parts.lsw = (ix1);      \
        (d)            = iw_u.value; \
    } while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d, v)          \
    do                               \
    {                                \
        ieee_double_shape_type sh_u; \
        sh_u.value     = (d);        \
        sh_u.parts.msw = (v);        \
        (d)            = sh_u.value; \
    } while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d, v)           \
    do                               \
    {                                \
        ieee_double_shape_type sl_u; \
        sl_u.value     = (d);        \
        sl_u.parts.lsw = (v);        \
        (d)            = sl_u.value; \
    } while (0)
=======
#define EXTRACT_WORDS(ix0, ix1, d)                                             \
  do {                                                                         \
    ieee_double_shape_type ew_u;                                               \
    ew_u.value = (d);                                                          \
    (ix0) = ew_u.parts.msw;                                                    \
    (ix1) = ew_u.parts.lsw;                                                    \
  } while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i, d)                                                    \
  do {                                                                         \
    ieee_double_shape_type gh_u;                                               \
    gh_u.value = (d);                                                          \
    (i) = gh_u.parts.msw;                                                      \
  } while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i, d)                                                     \
  do {                                                                         \
    ieee_double_shape_type gl_u;                                               \
    gl_u.value = (d);                                                          \
    (i) = gl_u.parts.lsw;                                                      \
  } while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d, ix0, ix1)                                              \
  do {                                                                         \
    ieee_double_shape_type iw_u;                                               \
    iw_u.parts.msw = (ix0);                                                    \
    iw_u.parts.lsw = (ix1);                                                    \
    (d) = iw_u.value;                                                          \
  } while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d, v)                                                    \
  do {                                                                         \
    ieee_double_shape_type sh_u;                                               \
    sh_u.value = (d);                                                          \
    sh_u.parts.msw = (v);                                                      \
    (d) = sh_u.value;                                                          \
  } while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d, v)                                                     \
  do {                                                                         \
    ieee_double_shape_type sl_u;                                               \
    sl_u.value = (d);                                                          \
    sl_u.parts.lsw = (v);                                                      \
    (d) = sl_u.value;                                                          \
  } while (0)
>>>>>>> master

/* A union which permits us to convert between a float and a 32 bit
   int.  */

<<<<<<< HEAD
typedef union
{
    float      value;
    __uint32_t word;
=======
typedef union {
  float value;
  __uint32_t word;
>>>>>>> master
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

<<<<<<< HEAD
#define GET_FLOAT_WORD(i, d)        \
    do                              \
    {                               \
        ieee_float_shape_type gf_u; \
        gf_u.value = (d);           \
        (i)        = gf_u.word;     \
    } while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d, i)        \
    do                              \
    {                               \
        ieee_float_shape_type sf_u; \
        sf_u.word = (i);            \
        (d)       = sf_u.value;     \
    } while (0)
=======
#define GET_FLOAT_WORD(i, d)                                                   \
  do {                                                                         \
    ieee_float_shape_type gf_u;                                                \
    gf_u.value = (d);                                                          \
    (i) = gf_u.word;                                                           \
  } while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d, i)                                                   \
  do {                                                                         \
    ieee_float_shape_type sf_u;                                                \
    sf_u.word = (i);                                                           \
    (d) = sf_u.value;                                                          \
  } while (0)
>>>>>>> master
