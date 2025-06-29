
/* @(#)e_scalb.c 5.1 93/09/24 */
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
 * __ieee754_scalb(x, fn) is provide for
 * passing various standard test suite. One
 * should use scalbn() instead.
 */

#include "fdlibm.h"

#ifdef _SCALB_INT
#ifdef __STDC__
double __ieee754_scalb(double x, int fn)
#else
double __ieee754_scalb(x, fn) double x;
<<<<<<< HEAD
int    fn;
=======
int fn;
>>>>>>> master
#endif
#else
#ifdef __STDC__
double __ieee754_scalb(double x, double fn)
#else
double __ieee754_scalb(x, fn) double x, fn;
#endif
#endif
{
#ifdef _SCALB_INT
<<<<<<< HEAD
    return scalbn(x, fn);
#else
    if (isnan(x) || isnan(fn)) return x * fn;
    if (!finite(fn)) {
        if (fn > 0.0)
            return x * fn;
        else
            return x / (-fn);
    }
    if (rint(fn) != fn) return (fn - fn) / (fn - fn);
    if (fn > 65000.0) return scalbn(x, 65000);
    if (-fn > 65000.0) return scalbn(x, -65000);
    return scalbn(x, (int)fn);
=======
  return scalbn(x, fn);
#else
  if (isnan(x) || isnan(fn))
    return x * fn;
  if (!finite(fn)) {
    if (fn > 0.0)
      return x * fn;
    else
      return x / (-fn);
  }
  if (rint(fn) != fn)
    return (fn - fn) / (fn - fn);
  if (fn > 65000.0)
    return scalbn(x, 65000);
  if (-fn > 65000.0)
    return scalbn(x, -65000);
  return scalbn(x, (int)fn);
>>>>>>> master
#endif
}
