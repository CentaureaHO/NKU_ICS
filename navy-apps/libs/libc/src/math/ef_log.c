/* ef_log.c -- float version of e_log.c.
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
    ln2_hi = 6.9313812256e-01, /* 0x3f317180 */
    ln2_lo = 9.0580006145e-06, /* 0x3717f7d1 */
<<<<<<< HEAD
    two25  = 3.355443200e+07,  /* 0x4c000000 */
    Lg1    = 6.6666668653e-01, /* 3F2AAAAB */
    Lg2    = 4.0000000596e-01, /* 3ECCCCCD */
    Lg3    = 2.8571429849e-01, /* 3E924925 */
    Lg4    = 2.2222198546e-01, /* 3E638E29 */
    Lg5    = 1.8183572590e-01, /* 3E3A3325 */
    Lg6    = 1.5313838422e-01, /* 3E1CD04F */
    Lg7    = 1.4798198640e-01; /* 3E178897 */
=======
    two25 = 3.355443200e+07,   /* 0x4c000000 */
    Lg1 = 6.6666668653e-01,    /* 3F2AAAAB */
    Lg2 = 4.0000000596e-01,    /* 3ECCCCCD */
    Lg3 = 2.8571429849e-01,    /* 3E924925 */
    Lg4 = 2.2222198546e-01,    /* 3E638E29 */
    Lg5 = 1.8183572590e-01,    /* 3E3A3325 */
    Lg6 = 1.5313838422e-01,    /* 3E1CD04F */
    Lg7 = 1.4798198640e-01;    /* 3E178897 */
>>>>>>> master

#ifdef __STDC__
static const float zero = 0.0;
#else
static float zero = 0.0;
#endif

#ifdef __STDC__
float __ieee754_logf(float x)
#else
<<<<<<< HEAD
float        __ieee754_logf(x) float x;
#endif
{
    float     hfsq, f, s, z, R, w, t1, t2, dk;
    __int32_t k, ix, i, j;

    GET_FLOAT_WORD(ix, x);

    k = 0;
    if (ix < 0x00800000) {                                /* x < 2**-126  */
        if ((ix & 0x7fffffff) == 0) return -two25 / zero; /* log(+-0)=-inf */
        if (ix < 0) return (x - x) / zero;                /* log(-#) = NaN */
        k -= 25;
        x *= two25; /* subnormal number, scale up x */
        GET_FLOAT_WORD(ix, x);
    }
    if (ix >= 0x7f800000) return x + x;
    k += (ix >> 23) - 127;
    ix &= 0x007fffff;
    i = (ix + (0x95f64 << 3)) & 0x800000;
    SET_FLOAT_WORD(x, ix | (i ^ 0x3f800000)); /* normalize x or x/2 */
    k += (i >> 23);
    f = x - (float)1.0;
    if ((0x007fffff & (15 + ix)) < 16) { /* |f| < 2**-20 */
        if (f == zero)
            if (k == 0)
                return zero;
            else
            {
                dk = (float)k;
                return dk * ln2_hi + dk * ln2_lo;
            }
        R = f * f * ((float)0.5 - (float)0.33333333333333333 * f);
        if (k == 0)
            return f - R;
        else
        {
            dk = (float)k;
            return dk * ln2_hi - ((R - dk * ln2_lo) - f);
        }
    }
    s  = f / ((float)2.0 + f);
    dk = (float)k;
    z  = s * s;
    i  = ix - (0x6147a << 3);
    w  = z * z;
    j  = (0x6b851 << 3) - ix;
    t1 = w * (Lg2 + w * (Lg4 + w * Lg6));
    t2 = z * (Lg1 + w * (Lg3 + w * (Lg5 + w * Lg7)));
    i |= j;
    R = t2 + t1;
    if (i > 0) {
        hfsq = (float)0.5 * f * f;
        if (k == 0)
            return f - (hfsq - s * (hfsq + R));
        else
            return dk * ln2_hi - ((hfsq - (s * (hfsq + R) + dk * ln2_lo)) - f);
    }
    else
    {
        if (k == 0)
            return f - s * (f - R);
        else
            return dk * ln2_hi - ((s * (f - R) - dk * ln2_lo) - f);
    }
=======
float __ieee754_logf(x) float x;
#endif
{
  float hfsq, f, s, z, R, w, t1, t2, dk;
  __int32_t k, ix, i, j;

  GET_FLOAT_WORD(ix, x);

  k = 0;
  if (ix < 0x00800000) { /* x < 2**-126  */
    if ((ix & 0x7fffffff) == 0)
      return -two25 / zero; /* log(+-0)=-inf */
    if (ix < 0)
      return (x - x) / zero; /* log(-#) = NaN */
    k -= 25;
    x *= two25; /* subnormal number, scale up x */
    GET_FLOAT_WORD(ix, x);
  }
  if (ix >= 0x7f800000)
    return x + x;
  k += (ix >> 23) - 127;
  ix &= 0x007fffff;
  i = (ix + (0x95f64 << 3)) & 0x800000;
  SET_FLOAT_WORD(x, ix | (i ^ 0x3f800000)); /* normalize x or x/2 */
  k += (i >> 23);
  f = x - (float)1.0;
  if ((0x007fffff & (15 + ix)) < 16) { /* |f| < 2**-20 */
    if (f == zero)
      if (k == 0)
        return zero;
      else {
        dk = (float)k;
        return dk * ln2_hi + dk * ln2_lo;
      }
    R = f * f * ((float)0.5 - (float)0.33333333333333333 * f);
    if (k == 0)
      return f - R;
    else {
      dk = (float)k;
      return dk * ln2_hi - ((R - dk * ln2_lo) - f);
    }
  }
  s = f / ((float)2.0 + f);
  dk = (float)k;
  z = s * s;
  i = ix - (0x6147a << 3);
  w = z * z;
  j = (0x6b851 << 3) - ix;
  t1 = w * (Lg2 + w * (Lg4 + w * Lg6));
  t2 = z * (Lg1 + w * (Lg3 + w * (Lg5 + w * Lg7)));
  i |= j;
  R = t2 + t1;
  if (i > 0) {
    hfsq = (float)0.5 * f * f;
    if (k == 0)
      return f - (hfsq - s * (hfsq + R));
    else
      return dk * ln2_hi - ((hfsq - (s * (hfsq + R) + dk * ln2_lo)) - f);
  } else {
    if (k == 0)
      return f - s * (f - R);
    else
      return dk * ln2_hi - ((s * (f - R) - dk * ln2_lo) - f);
  }
>>>>>>> master
}
