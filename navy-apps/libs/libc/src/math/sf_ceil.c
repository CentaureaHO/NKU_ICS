/* sf_ceil.c -- float version of s_ceil.c.
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
static const float huge = 1.0e30;
#else
static float huge = 1.0e30;
#endif

#ifdef __STDC__
float ceilf(float x)
#else
float ceilf(x) float x;
#endif
{
<<<<<<< HEAD
    __int32_t  i0, j0;
    __uint32_t i;
    GET_FLOAT_WORD(i0, x);
    j0 = ((i0 >> 23) & 0xff) - 0x7f;
    if (j0 < 23) {
        if (j0 < 0) {                    /* raise inexact if x != 0 */
            if (huge + x > (float)0.0) { /* return 0*sign(x) if |x|<1 */
                if (i0 < 0) {
                    i0 = 0x80000000;
                }
                else if (i0 != 0)
                {
                    i0 = 0x3f800000;
                }
            }
        }
        else
        {
            i = (0x007fffff) >> j0;
            if ((i0 & i) == 0) return x; /* x is integral */
            if (huge + x > (float)0.0) { /* raise inexact flag */
                if (i0 > 0) i0 += (0x00800000) >> j0;
                i0 &= (~i);
            }
        }
    }
    else
    {
        if (j0 == 0x80)
            return x + x; /* inf or NaN */
        else
            return x; /* x is integral */
    }
    SET_FLOAT_WORD(x, i0);
    return x;
=======
  __int32_t i0, j0;
  __uint32_t i;
  GET_FLOAT_WORD(i0, x);
  j0 = ((i0 >> 23) & 0xff) - 0x7f;
  if (j0 < 23) {
    if (j0 < 0) {                  /* raise inexact if x != 0 */
      if (huge + x > (float)0.0) { /* return 0*sign(x) if |x|<1 */
        if (i0 < 0) {
          i0 = 0x80000000;
        } else if (i0 != 0) {
          i0 = 0x3f800000;
        }
      }
    } else {
      i = (0x007fffff) >> j0;
      if ((i0 & i) == 0)
        return x;                  /* x is integral */
      if (huge + x > (float)0.0) { /* raise inexact flag */
        if (i0 > 0)
          i0 += (0x00800000) >> j0;
        i0 &= (~i);
      }
    }
  } else {
    if (j0 == 0x80)
      return x + x; /* inf or NaN */
    else
      return x; /* x is integral */
  }
  SET_FLOAT_WORD(x, i0);
  return x;
>>>>>>> master
}
