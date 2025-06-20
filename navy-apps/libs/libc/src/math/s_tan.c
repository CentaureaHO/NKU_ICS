
/* @(#)s_tan.c 5.1 93/09/24 */
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

FUNCTION
        <<tan>>, <<tanf>>---tangent

INDEX
tan
INDEX
tanf

ANSI_SYNOPSIS
        #include <math.h>
        double tan(double <[x]>);
        float tanf(float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double tan(<[x]>)
        double <[x]>;

        float tanf(<[x]>)
        float <[x]>;


DESCRIPTION
<<tan>> computes the tangent of the argument <[x]>.
Angles are specified in radians.

<<tanf>> is identical, save that it takes and returns <<float>> values.

RETURNS
The tangent of <[x]> is returned.

PORTABILITY
<<tan>> is ANSI. <<tanf>> is an extension.
*/

/* tan(x)
 * Return tangent function of x.
 *
 * kernel function:
 *	__kernel_tan		... tangent function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2	... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on
 *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2
 *	in [-pi/4 , +pi/4], and let n = k mod 4.
 *	We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0	       S	   C		 T
 *	    1	       C	  -S		-1/T
 *	    2	      -S	  -C		 T
 *	    3	      -C	   S		-1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *	TRIG(x) returns trig(x) nearly rounded
 */

#include "fdlibm.h"

#ifdef __STDC__
double tan(double x)
#else
double tan(x) double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
<<<<<<< HEAD
    double    y[2], z = 0.0;
    __int32_t n, ix;

    /* High word of x. */
    GET_HIGH_WORD(ix, x);

    /* |x| ~< pi/4 */
    ix &= 0x7fffffff;
    if (ix <= 0x3fe921fb) return __kernel_tan(x, z, 1);

    /* tan(Inf or NaN) is NaN */
    else if (ix >= 0x7ff00000)
        return x - x; /* NaN */

    /* argument reduction needed */
    else
    {
        n = __ieee754_rem_pio2(x, y);
        return __kernel_tan(y[0], y[1], 1 - ((n & 1) << 1)); /*   1 -- n even
                                                                    -1 -- n odd */
    }
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)tanf((float)x);
=======
  double y[2], z = 0.0;
  __int32_t n, ix;

  /* High word of x. */
  GET_HIGH_WORD(ix, x);

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if (ix <= 0x3fe921fb)
    return __kernel_tan(x, z, 1);

  /* tan(Inf or NaN) is NaN */
  else if (ix >= 0x7ff00000)
    return x - x; /* NaN */

  /* argument reduction needed */
  else {
    n = __ieee754_rem_pio2(x, y);
    return __kernel_tan(y[0], y[1],
                        1 - ((n & 1) << 1)); /*   1 -- n even
                                                                 -1 -- n odd */
  }
#else  /* defined (_DOUBLE_IS_32BITS) */
  return (double)tanf((float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}
