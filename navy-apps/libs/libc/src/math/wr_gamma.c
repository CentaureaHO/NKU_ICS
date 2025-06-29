
/* @(#)wr_gamma.c 5.1 93/09/24 */
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
 * wrapper double gamma_r(double x, int *signgamp)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define gamma_r _gamma_r_r
#endif

#if defined(_LIBM_REENT) || !defined(_REENT_ONLY)

#ifdef __STDC__
<<<<<<< HEAD
double gamma_r(_R1 double x, int* signgamp) /* wrapper lgamma_r */
#else
double gamma_r(_R2 x, signgamp) /* wrapper lgamma_r */
    _R3 double x;
int* signgamp;
=======
double gamma_r(_R1 double x, int *signgamp) /* wrapper lgamma_r */
#else
double gamma_r(_R2 x, signgamp) /* wrapper lgamma_r */
    _R3 double x;
int *signgamp;
>>>>>>> master
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
<<<<<<< HEAD
    return __ieee754_gamma_r(x, signgamp);
#else
    double y;
    y = __ieee754_gamma_r(x, signgamp);
    if (_LIB_VERSION == _IEEE_) return y;
    if (!finite(y) && finite(x)) {
        if (floor(x) == x && x <= 0.0)
            return __kernel_standard(_R4, x, x, 41); /* gamma pole */
        else
            return __kernel_standard(_R4, x, x, 40); /* gamma overflow */
    }
    else
        return y;
#endif
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)_gammaf_r_r(_R4, (float)x, signgamp);
=======
  return __ieee754_gamma_r(x, signgamp);
#else
  double y;
  y = __ieee754_gamma_r(x, signgamp);
  if (_LIB_VERSION == _IEEE_)
    return y;
  if (!finite(y) && finite(x)) {
    if (floor(x) == x && x <= 0.0)
      return __kernel_standard(_R4, x, x, 41); /* gamma pole */
    else
      return __kernel_standard(_R4, x, x, 40); /* gamma overflow */
  } else
    return y;
#endif
#else  /* defined (_DOUBLE_IS_32BITS) */
  return (double)_gammaf_r_r(_R4, (float)x, signgamp);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
