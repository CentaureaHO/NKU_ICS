
/* @(#)w_cosh.c 5.1 93/09/24 */
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
        <<cosh>>, <<coshf>>---hyperbolic cosine

ANSI_SYNOPSIS
        #include <math.h>
        double cosh(double <[x]>);
        float coshf(float <[x]>)

TRAD_SYNOPSIS
        #include <math.h>
        double cosh(<[x]>)
        double <[x]>;

        float coshf(<[x]>)
        float <[x]>;

DESCRIPTION

        <<cosh>> computes the hyperbolic cosine of the argument <[x]>.
        <<cosh(<[x]>)>> is defined as
        @ifinfo
        . (exp(x) + exp(-x))/2
        @end ifinfo
        @tex
        $${(e^x + e^{-x})} \over 2$$
        @end tex

        Angles are specified in radians.

        <<coshf>> is identical, save that it takes and returns <<float>>.

RETURNS
        The computed value is returned.  When the correct value would create
        an overflow,  <<cosh>> returns the value <<HUGE_VAL>> with the
        appropriate sign, and the global value <<errno>> is set to <<ERANGE>>.

        You can modify error handling for these functions using the
        function <<matherr>>.

PORTABILITY
        <<cosh>> is ANSI.
        <<coshf>> is an extension.

QUICKREF
        cosh ansi pure
        coshf - pure
*/

/*
 * wrapper cosh(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define cosh _cosh_r
#endif

#if defined(_LIBM_REENT) || !defined(_REENT_ONLY)
#ifdef __STDC__
double cosh(_R1 double x) /* wrapper cosh */
#else
double cosh(_R2 x) /* wrapper cosh */
    _R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
<<<<<<< HEAD
    return __ieee754_cosh(x);
#else
    double z;
    z = __ieee754_cosh(x);
    if (_LIB_VERSION == _IEEE_ || isnan(x)) return z;
    if (fabs(x) > 7.10475860073943863426e+02) {
        return __kernel_standard(_R4, x, x, 5); /* cosh overflow */
    }
    else
        return z;
#endif
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)_coshf_r(_R4, (float)x);
=======
  return __ieee754_cosh(x);
#else
  double z;
  z = __ieee754_cosh(x);
  if (_LIB_VERSION == _IEEE_ || isnan(x))
    return z;
  if (fabs(x) > 7.10475860073943863426e+02) {
    return __kernel_standard(_R4, x, x, 5); /* cosh overflow */
  } else
    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
  return (double)_coshf_r(_R4, (float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
