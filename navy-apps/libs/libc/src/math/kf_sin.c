/* kf_sin.c -- float version of k_sin.c
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
    half = 5.0000000000e-01,  /* 0x3f000000 */
    S1   = -1.6666667163e-01, /* 0xbe2aaaab */
    S2   = 8.3333337680e-03,  /* 0x3c088889 */
    S3   = -1.9841270114e-04, /* 0xb9500d01 */
    S4   = 2.7557314297e-06,  /* 0x3638ef1b */
    S5   = -2.5050759689e-08, /* 0xb2d72f34 */
    S6   = 1.5896910177e-10;  /* 0x2f2ec9d3 */
=======
    half = 5.0000000000e-01, /* 0x3f000000 */
    S1 = -1.6666667163e-01,  /* 0xbe2aaaab */
    S2 = 8.3333337680e-03,   /* 0x3c088889 */
    S3 = -1.9841270114e-04,  /* 0xb9500d01 */
    S4 = 2.7557314297e-06,   /* 0x3638ef1b */
    S5 = -2.5050759689e-08,  /* 0xb2d72f34 */
    S6 = 1.5896910177e-10;   /* 0x2f2ec9d3 */
>>>>>>> master

#ifdef __STDC__
float __kernel_sinf(float x, float y, int iy)
#else
float __kernel_sinf(x, y, iy) float x, y;
<<<<<<< HEAD
int   iy; /* iy=0 if y is zero */
#endif
{
    float     z, r, v;
    __int32_t ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;    /* high word of x */
    if (ix < 0x32000000) /* |x| < 2**-27 */
    {
        if ((int)x == 0) return x;
    } /* generate inexact */
    z = x * x;
    v = z * x;
    r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
    if (iy == 0)
        return x + v * (S1 + z * r);
    else
        return x - ((z * (half * y - v * r) - y) - v * S1);
=======
int iy; /* iy=0 if y is zero */
#endif
{
  float z, r, v;
  __int32_t ix;
  GET_FLOAT_WORD(ix, x);
  ix &= 0x7fffffff;    /* high word of x */
  if (ix < 0x32000000) /* |x| < 2**-27 */
  {
    if ((int)x == 0)
      return x;
  } /* generate inexact */
  z = x * x;
  v = z * x;
  r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
  if (iy == 0)
    return x + v * (S1 + z * r);
  else
    return x - ((z * (half * y - v * r) - y) - v * S1);
>>>>>>> master
}
