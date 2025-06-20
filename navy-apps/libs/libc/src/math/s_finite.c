
/* @(#)s_finite.c 5.1 93/09/24 */
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
 * finite(x) returns 1 is x is finite, else 0;
 * no branching!
 */

#include "fdlibm.h"

#ifdef __STDC__
int finite(double x)
#else
int finite(x) double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
<<<<<<< HEAD
    __int32_t hx;
    GET_HIGH_WORD(hx, x);
    return (int)((__uint32_t)((hx & 0x7fffffff) - 0x7ff00000) >> 31);
#else  /* defined (_DOUBLE_IS_32BITS) */
    return finitef((float)x);
=======
  __int32_t hx;
  GET_HIGH_WORD(hx, x);
  return (int)((__uint32_t)((hx & 0x7fffffff) - 0x7ff00000) >> 31);
#else  /* defined (_DOUBLE_IS_32BITS) */
  return finitef((float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}
