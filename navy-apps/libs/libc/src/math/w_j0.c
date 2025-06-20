
/* @(#)w_j0.c 5.1 93/09/24 */
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
<<jN>>,<<jNf>>,<<yN>>,<<yNf>>---Bessel functions

INDEX
j0
INDEX
j0f
INDEX
j1
INDEX
j1f
INDEX
jn
INDEX
jnf
INDEX
y0
INDEX
y0f
INDEX
y1
INDEX
y1f
INDEX
yn
INDEX
ynf

ANSI_SYNOPSIS
#include <math.h>
double j0(double <[x]>);
float j0f(float <[x]>);
double j1(double <[x]>);
float j1f(float <[x]>);
double jn(int <[n]>, double <[x]>);
float jnf(int <[n]>, float <[x]>);
double y0(double <[x]>);
float y0f(float <[x]>);
double y1(double <[x]>);
float y1f(float <[x]>);
double yn(int <[n]>, double <[x]>);
float ynf(int <[n]>, float <[x]>);

TRAD_SYNOPSIS
#include <math.h>

double j0(<[x]>)
double <[x]>;
float j0f(<[x]>)
float <[x]>;
double j1(<[x]>)
double <[x]>;
float j1f(<[x]>)
float <[x]>;
double jn(<[n]>, <[x]>)
int <[n]>;
double <[x]>;
float jnf(<[n]>, <[x]>)
int <[n]>;
float <[x]>;

double y0(<[x]>)
double <[x]>;
float y0f(<[x]>)
float <[x]>;
double y1(<[x]>)
double <[x]>;
float y1f(<[x]>)
float <[x]>;
double yn(<[n]>, <[x]>)
int <[n]>;
double <[x]>;
float ynf(<[n]>, <[x]>)
int <[n]>;
float <[x]>;

DESCRIPTION
The Bessel functions are a family of functions that solve the
differential equation
@ifinfo
.  2               2    2
. x  y'' + xy' + (x  - p )y  = 0
@end ifinfo
@tex
$$x^2{d^2y\over dx^2} + x{dy\over dx} + (x^2-p^2)y = 0$$
@end tex
These functions have many applications in engineering and physics.

<<jn>> calculates the Bessel function of the first kind of order
<[n]>.  <<j0>> and <<j1>> are special cases for order 0 and order
1 respectively.

Similarly, <<yn>> calculates the Bessel function of the second kind of
order <[n]>, and <<y0>> and <<y1>> are special cases for order 0 and
1.

<<jnf>>, <<j0f>>, <<j1f>>, <<ynf>>, <<y0f>>, and <<y1f>> perform the
same calculations, but on <<float>> rather than <<double>> values.

RETURNS
The value of each Bessel function at <[x]> is returned.

PORTABILITY
None of the Bessel functions are in ANSI C.
*/

/*
 * wrapper j0(double x), y0(double x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define j0 _j0_r
#define y0 _y0_r
#endif

#if defined(_LIBM_REENT) || !defined(_REENT_ONLY)

#ifdef __STDC__
double j0(_R1 double x) /* wrapper j0 */
#else
double j0(_R2 x) /* wrapper j0 */
    _R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
<<<<<<< HEAD
    return __ieee754_j0(x);
#else
    double z = __ieee754_j0(x);
    if (_LIB_VERSION == _IEEE_ || isnan(x)) return z;
    if (fabs(x) > X_TLOSS) {
        return __kernel_standard(_R4, x, x, 34); /* j0(|x|>X_TLOSS) */
    }
    else
        return z;
#endif
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)_j0f_r(_R4, (float)x);
=======
  return __ieee754_j0(x);
#else
  double z = __ieee754_j0(x);
  if (_LIB_VERSION == _IEEE_ || isnan(x))
    return z;
  if (fabs(x) > X_TLOSS) {
    return __kernel_standard(_R4, x, x, 34); /* j0(|x|>X_TLOSS) */
  } else
    return z;
#endif
#else  /* defined (_DOUBLE_IS_32BITS) */
  return (double)_j0f_r(_R4, (float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#ifdef __STDC__
double y0(_R1 double x) /* wrapper y0 */
#else
double y0(_R2 x) /* wrapper y0 */
    _R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
<<<<<<< HEAD
    return __ieee754_y0(x);
#else
    double z;
    z = __ieee754_y0(x);
    if (_LIB_VERSION == _IEEE_ || isnan(x)) return z;
    if (x <= 0.0) {
        if (x == 0.0) /* d= -one/(x-x); */
            return __kernel_standard(_R4, x, x, 8);
        else
            /* d = zero/(x-x); */
            return __kernel_standard(_R4, x, x, 9);
    }
    if (x > X_TLOSS) {
        return __kernel_standard(_R4, x, x, 35); /* y0(x>X_TLOSS) */
    }
    else
        return z;
#endif
#else  /* defined (_DOUBLE_IS_32BITS) */
    return (double)_y0f_r(_R4, (float)x);
=======
  return __ieee754_y0(x);
#else
  double z;
  z = __ieee754_y0(x);
  if (_LIB_VERSION == _IEEE_ || isnan(x))
    return z;
  if (x <= 0.0) {
    if (x == 0.0) /* d= -one/(x-x); */
      return __kernel_standard(_R4, x, x, 8);
    else
      /* d = zero/(x-x); */
      return __kernel_standard(_R4, x, x, 9);
  }
  if (x > X_TLOSS) {
    return __kernel_standard(_R4, x, x, 35); /* y0(x>X_TLOSS) */
  } else
    return z;
#endif
#else  /* defined (_DOUBLE_IS_32BITS) */
  return (double)_y0f_r(_R4, (float)x);
>>>>>>> master
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
