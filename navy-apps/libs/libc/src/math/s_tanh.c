
/* @(#)s_tanh.c 5.1 93/09/24 */
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
        <<tanh>>, <<tanhf>>---hyperbolic tangent

INDEX
tanh
INDEX
tanhf

ANSI_SYNOPSIS
        #include <math.h>
        double tanh(double <[x]>);
        float tanhf(float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double tanh(<[x]>)
        double <[x]>;

        float tanhf(<[x]>)
        float <[x]>;


DESCRIPTION

<<tanh>> computes the hyperbolic tangent of
the argument <[x]>.  Angles are specified in radians.

<<tanh(<[x]>)>> is defined as
. sinh(<[x]>)/cosh(<[x]>)

<<tanhf>> is identical, save that it takes and returns <<float>> values.

RETURNS
The hyperbolic tangent of <[x]> is returned.

PORTABILITY
<<tanh>> is ANSI C.  <<tanhf>> is an extension.

*/

/* Tanh(x)
 * Return the Hyperbolic Tangent of x
 *
 * Method :
 *				       x    -x
 *				      e  - e
 *	0. tanh(x) is defined to be -----------
 *				       x    -x
 *				      e  + e
 *	1. reduce x to non-negative by tanh(-x) = -tanh(x).
 *	2.  0      <= x <= 2**-55 : tanh(x) := x*(one+x)
 *					        -t
 *	    2**-55 <  x <=  1     : tanh(x) := -----; t = expm1(-2x)
 *					       t + 2
 *						     2
 *	    1      <= x <=  22.0  : tanh(x) := 1-  ----- ; t=expm1(2x)
 *						   t + 2
 *	    22.0   <  x <= INF    : tanh(x) := 1.
 *
 * Special cases:
 *	tanh(NaN) is NaN;
 *	only tanh(0)=0 is exact for finite argument.
 */

#include "fdlibm.h"

#ifdef __STDC__
static const double one = 1.0, two = 2.0, tiny = 1.0e-300;
#else
static double one = 1.0, two = 2.0, tiny = 1.0e-300;
#endif

#ifdef __STDC__
double tanh(double x)
#else
double tanh(x) double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
<<<<<<< HEAD
    double    t, z;
    __int32_t jx, ix;

    /* High word of |x|. */
    GET_HIGH_WORD(jx, x);
    ix = jx & 0x7fffffff;

    /* x is INF or NaN */
    if (ix >= 0x7ff00000) {
        if (jx >= 0)
            return one / x + one; /* tanh(+-inf)=+-1 */
        else
            return one / x - one; /* tanh(NaN) = NaN */
    }

    /* |x| < 22 */
    if (ix < 0x40360000) {        /* |x|<22 */
        if (ix < 0x3c800000)      /* |x|<2**-55 */
            return x * (one + x); /* tanh(small) = small */
        if (ix >= 0x3ff00000) {   /* |x|>=1  */
            t = expm1(two * fabs(x));
            z = one - two / (t + two);
        }
        else
        {
            t = expm1(-two * fabs(x));
            z = -t / (t + two);
        }
        /* |x| > 22, return +-1 */
    }
    else
    {
        z = one - tiny; /* raised inexact flag */
    }
    return (jx >= 0) ? z : -z;
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)tanhf((float)x);
=======
  double t, z;
  __int32_t jx, ix;

  /* High word of |x|. */
  GET_HIGH_WORD(jx, x);
  ix = jx & 0x7fffffff;

  /* x is INF or NaN */
  if (ix >= 0x7ff00000) {
    if (jx >= 0)
      return one / x + one; /* tanh(+-inf)=+-1 */
    else
      return one / x - one; /* tanh(NaN) = NaN */
  }

  /* |x| < 22 */
  if (ix < 0x40360000) {    /* |x|<22 */
    if (ix < 0x3c800000)    /* |x|<2**-55 */
      return x * (one + x); /* tanh(small) = small */
    if (ix >= 0x3ff00000) { /* |x|>=1  */
      t = expm1(two * fabs(x));
      z = one - two / (t + two);
    } else {
      t = expm1(-two * fabs(x));
      z = -t / (t + two);
    }
    /* |x| > 22, return +-1 */
  } else {
    z = one - tiny; /* raised inexact flag */
  }
  return (jx >= 0) ? z : -z;
#else  /* defined (_DOUBLE_IS_32BITS) */
  return (double)tanhf((float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}
