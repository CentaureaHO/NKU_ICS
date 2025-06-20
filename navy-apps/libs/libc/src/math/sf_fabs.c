/* sf_fabs.c -- float version of s_fabs.c.
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

/*
 * fabsf(x) returns the absolute value of x.
 */

#include "fdlibm.h"

#ifdef __STDC__
float fabsf(float x)
#else
float fabsf(x) float x;
#endif
{
<<<<<<< HEAD
    __uint32_t ix;
    GET_FLOAT_WORD(ix, x);
    SET_FLOAT_WORD(x, ix & 0x7fffffff);
    return x;
=======
  __uint32_t ix;
  GET_FLOAT_WORD(ix, x);
  SET_FLOAT_WORD(x, ix & 0x7fffffff);
  return x;
>>>>>>> master
}
