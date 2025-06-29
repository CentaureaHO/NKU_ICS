/* sf_signif.c -- float version of s_signif.c.
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
float significandf(float x)
#else
float significandf(x) float x;
#endif
{
<<<<<<< HEAD
    return __ieee754_scalbf(x, (float)-ilogbf(x));
=======
  return __ieee754_scalbf(x, (float)-ilogbf(x));
>>>>>>> master
}
