
/* @(#)s_asinh.c 5.1 93/09/24 */
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
        <<asinh>>, <<asinhf>>---inverse hyperbolic sine

INDEX
        asinh
INDEX
        asinhf

ANSI_SYNOPSIS
        #include <math.h>
        double asinh(double <[x]>);
        float asinhf(float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double asinh(<[x]>)
        double <[x]>;

        float asinhf(<[x]>)
        float <[x]>;

DESCRIPTION
<<asinh>> calculates the inverse hyperbolic sine of <[x]>.
<<asinh>> is defined as
@ifinfo
. sgn(<[x]>) * log(abs(<[x]>) + sqrt(1+<[x]>*<[x]>))
@end ifinfo
@tex
$$sign(x) \times ln\Bigl(|x| + \sqrt{1+x^2}\Bigr)$$
@end tex

<<asinhf>> is identical, other than taking and returning floats.

RETURNS
<<asinh>> and <<asinhf>> return the calculated value.

PORTABILITY
Neither <<asinh>> nor <<asinhf>> are ANSI C.

*/

/* asinh(x)
 * Method :
 *	Based on
 *		asinh(x) = sign(x) * log [ |x| + sqrt(x*x+1) ]
 *	we have
 *	asinh(x) := x  if  1+x*x=1,
 *		 := sign(x)*(log(x)+ln2)) for large |x|, else
 *		 := sign(x)*log(2|x|+1/(|x|+sqrt(x*x+1))) if|x|>2, else
 *		 := sign(x)*log1p(|x| + x^2/(1 + sqrt(1+x^2)))
 */

#include "fdlibm.h"

#ifdef __STDC__
static const double
#else
static double
#endif
<<<<<<< HEAD
    one  = 1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
    ln2  = 6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
=======
    one = 1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
    ln2 = 6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
>>>>>>> master
    huge = 1.00000000000000000000e+300;

#ifdef __STDC__
double asinh(double x)
#else
double asinh(x) double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
<<<<<<< HEAD
    double    t, w;
    __int32_t hx, ix;
    GET_HIGH_WORD(hx, x);
    ix = hx & 0x7fffffff;
    if (ix >= 0x7ff00000) return x + x; /* x is inf or NaN */
    if (ix < 0x3e300000) {              /* |x|<2**-28 */
        if (huge + x > one) return x;   /* return x inexact except 0 */
    }
    if (ix > 0x41b00000) { /* |x| > 2**28 */
        w = __ieee754_log(fabs(x)) + ln2;
    }
    else if (ix > 0x40000000)
    { /* 2**28 > |x| > 2.0 */
        t = fabs(x);
        w = __ieee754_log(2.0 * t + one / (__ieee754_sqrt(x * x + one) + t));
    }
    else
    { /* 2.0 > |x| > 2**-28 */
        t = x * x;
        w = log1p(fabs(x) + t / (one + __ieee754_sqrt(one + t)));
    }
    if (hx > 0)
        return w;
    else
        return -w;
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)asinhf((float)x);
=======
  double t, w;
  __int32_t hx, ix;
  GET_HIGH_WORD(hx, x);
  ix = hx & 0x7fffffff;
  if (ix >= 0x7ff00000)
    return x + x;        /* x is inf or NaN */
  if (ix < 0x3e300000) { /* |x|<2**-28 */
    if (huge + x > one)
      return x; /* return x inexact except 0 */
  }
  if (ix > 0x41b00000) { /* |x| > 2**28 */
    w = __ieee754_log(fabs(x)) + ln2;
  } else if (ix > 0x40000000) { /* 2**28 > |x| > 2.0 */
    t = fabs(x);
    w = __ieee754_log(2.0 * t + one / (__ieee754_sqrt(x * x + one) + t));
  } else { /* 2.0 > |x| > 2**-28 */
    t = x * x;
    w = log1p(fabs(x) + t / (one + __ieee754_sqrt(one + t)));
  }
  if (hx > 0)
    return w;
  else
    return -w;
#else  /* defined (_DOUBLE_IS_32BITS) */
  return (double)asinhf((float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}
