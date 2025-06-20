/* sf_logb.c -- float version of s_logb.c.
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
float logbf(float x)
#else
float logbf(x) float x;
#endif
{
<<<<<<< HEAD
    __int32_t ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff; /* high |x| */
    if (ix == 0) return (float)-1.0 / fabsf(x);
    if (ix >= 0x7f800000) return x * x;
    if ((ix >>= 23) == 0) /* IEEE 754 logb */
        return -126.0;
    else
        return (float)(ix - 127);
=======
  __int32_t ix;
  GET_FLOAT_WORD(ix, x);
  ix &= 0x7fffffff; /* high |x| */
  if (ix == 0)
    return (float)-1.0 / fabsf(x);
  if (ix >= 0x7f800000)
    return x * x;
  if ((ix >>= 23) == 0) /* IEEE 754 logb */
    return -126.0;
  else
    return (float)(ix - 127);
>>>>>>> master
}
