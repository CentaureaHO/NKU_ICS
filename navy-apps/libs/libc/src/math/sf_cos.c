/* sf_cos.c -- float version of s_cos.c.
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
static const float one = 1.0;
#else
static float one = 1.0;
#endif

#ifdef __STDC__
float cosf(float x)
#else
float cosf(x) float x;
#endif
{
<<<<<<< HEAD
    float     y[2], z = 0.0;
    __int32_t n, ix;

    GET_FLOAT_WORD(ix, x);

    /* |x| ~< pi/4 */
    ix &= 0x7fffffff;
    if (ix <= 0x3f490fd8) return __kernel_cosf(x, z);

    /* cos(Inf or NaN) is NaN */
    else if (ix >= 0x7f800000)
        return x - x;

    /* argument reduction needed */
    else
    {
        n = __ieee754_rem_pio2f(x, y);
        switch (n & 3)
        {
            case 0: return __kernel_cosf(y[0], y[1]);
            case 1: return -__kernel_sinf(y[0], y[1], 1);
            case 2: return -__kernel_cosf(y[0], y[1]);
            default: return __kernel_sinf(y[0], y[1], 1);
        }
    }
=======
  float y[2], z = 0.0;
  __int32_t n, ix;

  GET_FLOAT_WORD(ix, x);

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if (ix <= 0x3f490fd8)
    return __kernel_cosf(x, z);

  /* cos(Inf or NaN) is NaN */
  else if (ix >= 0x7f800000)
    return x - x;

  /* argument reduction needed */
  else {
    n = __ieee754_rem_pio2f(x, y);
    switch (n & 3) {
    case 0:
      return __kernel_cosf(y[0], y[1]);
    case 1:
      return -__kernel_sinf(y[0], y[1], 1);
    case 2:
      return -__kernel_cosf(y[0], y[1]);
    default:
      return __kernel_sinf(y[0], y[1], 1);
    }
  }
>>>>>>> master
}
