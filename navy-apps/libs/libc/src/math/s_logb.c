
/* @(#)s_logb.c 5.1 93/09/24 */
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
 * double logb(x)
 * IEEE 754 logb. Included to pass IEEE test suite. Not recommend.
 * Use ilogb instead.
 */

#include "fdlibm.h"

#ifdef __STDC__
double logb(double x)
#else
double logb(x) double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
<<<<<<< HEAD
    __int32_t lx, ix;
    EXTRACT_WORDS(ix, lx, x);
    ix &= 0x7fffffff; /* high |x| */
    if ((ix | lx) == 0) return -1.0 / fabs(x);
    if (ix >= 0x7ff00000) return x * x;
    if ((ix >>= 20) == 0) /* IEEE 754 logb */
        return -1022.0;
    else
        return (double)(ix - 1023);
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)logb((float)x);
=======
  __int32_t lx, ix;
  EXTRACT_WORDS(ix, lx, x);
  ix &= 0x7fffffff; /* high |x| */
  if ((ix | lx) == 0)
    return -1.0 / fabs(x);
  if (ix >= 0x7ff00000)
    return x * x;
  if ((ix >>= 20) == 0) /* IEEE 754 logb */
    return -1022.0;
  else
    return (double)(ix - 1023);
#else  /* defined (_DOUBLE_IS_32BITS) */
  return (double)logb((float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}
