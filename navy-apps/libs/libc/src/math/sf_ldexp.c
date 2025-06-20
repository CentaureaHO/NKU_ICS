/* sf_ldexp.c -- float version of s_ldexp.c.
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
#include <errno.h>

#ifdef _LIBM_REENT
#define ldexpf _ldexpf_r
#endif

#if defined(_LIBM_REENT) || !defined(_REENT_ONLY)

#ifdef __STDC__
float ldexpf(_R1 float value, int exp)
#else
float ldexpf(_R2 value, exp) _R3 float value;
int exp;
#endif
{
<<<<<<< HEAD
    if (!finitef(value) || value == (float)0.0) return value;
    value                                                   = scalbnf(value, exp);
    if (!finitef(value) || value == (float)0.0) _R4->_errno = ERANGE;
    return value;
=======
  if (!finitef(value) || value == (float)0.0)
    return value;
  value = scalbnf(value, exp);
  if (!finitef(value) || value == (float)0.0)
    _R4->_errno = ERANGE;
  return value;
>>>>>>> master
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
