/* sf_modf.c -- float version of s_modf.c.
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
<<<<<<< HEAD
float modff(float x, float* iptr)
=======
float modff(float x, float *iptr)
>>>>>>> master
#else
float modff(x, iptr) float x, *iptr;
#endif
{
<<<<<<< HEAD
    __int32_t  i0, j0;
    __uint32_t i;
    GET_FLOAT_WORD(i0, x);
    j0 = ((i0 >> 23) & 0xff) - 0x7f;                /* exponent of x */
    if (j0 < 23) {                                  /* integer part in x */
        if (j0 < 0) {                               /* |x|<1 */
            SET_FLOAT_WORD(*iptr, i0 & 0x80000000); /* *iptr = +-0 */
            return x;
        }
        else
        {
            i = (0x007fffff) >> j0;
            if ((i0 & i) == 0) { /* x is integral */
                __uint32_t ix;
                *iptr = x;
                GET_FLOAT_WORD(ix, x);
                SET_FLOAT_WORD(x, ix & 0x80000000); /* return +-0 */
                return x;
            }
            else
            {
                SET_FLOAT_WORD(*iptr, i0 & (~i));
                return x - *iptr;
            }
        }
    }
    else
    { /* no fraction part */
        __uint32_t ix;
        *iptr = x * one;
        GET_FLOAT_WORD(ix, x);
        SET_FLOAT_WORD(x, ix & 0x80000000); /* return +-0 */
        return x;
    }
=======
  __int32_t i0, j0;
  __uint32_t i;
  GET_FLOAT_WORD(i0, x);
  j0 = ((i0 >> 23) & 0xff) - 0x7f;            /* exponent of x */
  if (j0 < 23) {                              /* integer part in x */
    if (j0 < 0) {                             /* |x|<1 */
      SET_FLOAT_WORD(*iptr, i0 & 0x80000000); /* *iptr = +-0 */
      return x;
    } else {
      i = (0x007fffff) >> j0;
      if ((i0 & i) == 0) { /* x is integral */
        __uint32_t ix;
        *iptr = x;
        GET_FLOAT_WORD(ix, x);
        SET_FLOAT_WORD(x, ix & 0x80000000); /* return +-0 */
        return x;
      } else {
        SET_FLOAT_WORD(*iptr, i0 & (~i));
        return x - *iptr;
      }
    }
  } else { /* no fraction part */
    __uint32_t ix;
    *iptr = x * one;
    GET_FLOAT_WORD(ix, x);
    SET_FLOAT_WORD(x, ix & 0x80000000); /* return +-0 */
    return x;
  }
>>>>>>> master
}
