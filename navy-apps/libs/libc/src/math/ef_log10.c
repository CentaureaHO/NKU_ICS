/* ef_log10.c -- float version of e_log10.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include "fdlibm.h"

#ifdef __STDC__
static const float
#else
static float
#endif
<<<<<<< HEAD
    two25     = 3.3554432000e+07, /* 0x4c000000 */
    ivln10    = 4.3429449201e-01, /* 0x3ede5bd9 */
=======
    two25 = 3.3554432000e+07,     /* 0x4c000000 */
    ivln10 = 4.3429449201e-01,    /* 0x3ede5bd9 */
>>>>>>> master
    log10_2hi = 3.0102920532e-01, /* 0x3e9a2080 */
    log10_2lo = 7.9034151668e-07; /* 0x355427db */

#ifdef __STDC__
static const float zero = 0.0;
#else
static float zero = 0.0;
#endif

#ifdef __STDC__
float __ieee754_log10f(float x)
#else
<<<<<<< HEAD
float        __ieee754_log10f(x) float x;
#endif
{
    float     y, z;
    __int32_t i, k, hx;

    GET_FLOAT_WORD(hx, x);

    k = 0;
    if (hx < 0x00800000) {                                /* x < 2**-126  */
        if ((hx & 0x7fffffff) == 0) return -two25 / zero; /* log(+-0)=-inf */
        if (hx < 0) return (x - x) / zero;                /* log(-#) = NaN */
        k -= 25;
        x *= two25; /* subnormal number, scale up x */
        GET_FLOAT_WORD(hx, x);
    }
    if (hx >= 0x7f800000) return x + x;
    k += (hx >> 23) - 127;
    i  = ((__uint32_t)k & 0x80000000) >> 31;
    hx = (hx & 0x007fffff) | ((0x7f - i) << 23);
    y  = (float)(k + i);
    SET_FLOAT_WORD(x, hx);
    z = y * log10_2lo + ivln10 * __ieee754_logf(x);
    return z + y * log10_2hi;
=======
float __ieee754_log10f(x) float x;
#endif
{
  float y, z;
  __int32_t i, k, hx;

  GET_FLOAT_WORD(hx, x);

  k = 0;
  if (hx < 0x00800000) { /* x < 2**-126  */
    if ((hx & 0x7fffffff) == 0)
      return -two25 / zero; /* log(+-0)=-inf */
    if (hx < 0)
      return (x - x) / zero; /* log(-#) = NaN */
    k -= 25;
    x *= two25; /* subnormal number, scale up x */
    GET_FLOAT_WORD(hx, x);
  }
  if (hx >= 0x7f800000)
    return x + x;
  k += (hx >> 23) - 127;
  i = ((__uint32_t)k & 0x80000000) >> 31;
  hx = (hx & 0x007fffff) | ((0x7f - i) << 23);
  y = (float)(k + i);
  SET_FLOAT_WORD(x, hx);
  z = y * log10_2lo + ivln10 * __ieee754_logf(x);
  return z + y * log10_2hi;
>>>>>>> master
}
