/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/

/* Please send bug reports to
        David M. Gay
        AT&T Bell Laboratories, Room 2C-463
        600 Mountain Avenue
        Murray Hill, NJ 07974-2070
        U.S.A.
        dmg@research.att.com or research!dmg
 */

/* strtod for IEEE-, VAX-, and IBM-arithmetic machines.
 *
 * This strtod returns a nearest machine number to the input decimal
 * string (or sets errno to ERANGE).  With IEEE arithmetic, ties are
 * broken by the IEEE round-even rule.  Otherwise ties are broken by
 * biased rounding (add half and chop).
 *
 * Inspired loosely by William D. Clinger's paper "How to Read Floating
 * Point Numbers Accurately" [Proc. ACM SIGPLAN '90, pp. 92-101].
 *
 * Modifications:
 *
 *	1. We only require IEEE, IBM, or VAX double-precision
 *		arithmetic (not IEEE double-extended).
 *	2. We get by with floating-point arithmetic in a case that
 *		Clinger missed -- when we're computing d * 10^n
 *		for a small integer d and the integer n is not too
 *		much larger than 22 (the maximum integer k for which
 *		we can represent 10^k exactly), we may be able to
 *		compute (d*10^k) * 10^(e-k) with just one roundoff.
 *	3. Rather than a bit-at-a-time adjustment of the binary
 *		result in the hard case, we use floating-point
 *		arithmetic to determine the adjustment to within
 *		one bit; only in really hard cases do we need to
 *		compute a second residual.
 *	4. Because of 3., we don't need a large table of powers of 10
 *		for ten-to-e (just some small tables, e.g. of 10^k
 *		for 0 <= k <= 22).
 */

/*
 * #define IEEE_8087 for IEEE-arithmetic machines where the least
 *	significant byte has the lowest address.
 * #define IEEE_MC68k for IEEE-arithmetic machines where the most
 *	significant byte has the lowest address.
 * #define Sudden_Underflow for IEEE-format machines without gradual
 *	underflow (i.e., that flush to zero on underflow).
 * #define IBM for IBM mainframe-style floating-point arithmetic.
 * #define VAX for VAX-style floating-point arithmetic.
 * #define Unsigned_Shifts if >> does treats its left operand as unsigned.
 * #define No_leftright to omit left-right logic in fast floating-point
 *	computation of dtoa.
 * #define Check_FLT_ROUNDS if FLT_ROUNDS can assume the values 2 or 3.
 * #define RND_PRODQUOT to use rnd_prod and rnd_quot (assembly routines
 *	that use extended-precision instructions to compute rounded
 *	products and quotients) with IBM.
 * #define ROUND_BIASED for IEEE-format with biased rounding.
 * #define Inaccurate_Divide for IEEE-format with correctly rounded
 *	products but inaccurate quotients, e.g., for Intel i860.
 * #define Just_16 to store 16 bits per 32-bit long when doing high-precision
 *	integer arithmetic.  Whether this speeds things up or slows things
 *	down depends on the machine and the number being converted.
 */

#include "mprec.h"
#include <_ansi.h>
#include <reent.h>
#include <stdlib.h>
#include <string.h>

/* reent.c knows this value */
#define _Kmax 15

<<<<<<< HEAD
_Bigint* _DEFUN(Balloc, (ptr, k), struct _reent* ptr _AND int k)
{
    int      x;
    _Bigint* rv;

    if (ptr->_freelist == NULL) {
        ptr->_freelist = (struct _Bigint**)_calloc_r(ptr, sizeof(struct _Bigint*), (size_t)(_Kmax + 1));
        if (ptr->_freelist == NULL) {
            return NULL;
        }
=======
_Bigint *_DEFUN(Balloc, (ptr, k), struct _reent *ptr _AND int k) {
  int x;
  _Bigint *rv;

  if (ptr->_freelist == NULL) {
    ptr->_freelist = (struct _Bigint **)_calloc_r(ptr, sizeof(struct _Bigint *),
                                                  (size_t)(_Kmax + 1));
    if (ptr->_freelist == NULL) {
      return NULL;
>>>>>>> master
    }
  }

<<<<<<< HEAD
    if (rv = ptr->_freelist[k]) {
        ptr->_freelist[k] = rv->_next;
    }
    else
    {
        x  = 1 << k;
        rv = (_Bigint*)_calloc_r(ptr, sizeof(_Bigint) + (x - 1), (size_t)sizeof(long));
        if (rv == NULL) return NULL;
        rv->_k      = k;
        rv->_maxwds = x;

        rv->_reclaim          = ptr->_freelist[_Kmax];
        ptr->_freelist[_Kmax] = rv;
    }
    rv->_sign = rv->_wds = 0;
    return rv;
}

void _DEFUN(Bfree, (ptr, v), struct _reent* ptr _AND _Bigint* v)
{
    if (v) {
        v->_next              = ptr->_freelist[v->_k];
        ptr->_freelist[v->_k] = v;
    }
}

_Bigint* _DEFUN(multadd, (ptr, b, m, a), struct _reent* ptr _AND _Bigint* b _AND int m _AND int a)
{
    int            i, wds;
    unsigned long *x, y;
=======
  if (rv = ptr->_freelist[k]) {
    ptr->_freelist[k] = rv->_next;
  } else {
    x = 1 << k;
    rv = (_Bigint *)_calloc_r(ptr, sizeof(_Bigint) + (x - 1),
                              (size_t)sizeof(long));
    if (rv == NULL)
      return NULL;
    rv->_k = k;
    rv->_maxwds = x;

    rv->_reclaim = ptr->_freelist[_Kmax];
    ptr->_freelist[_Kmax] = rv;
  }
  rv->_sign = rv->_wds = 0;
  return rv;
}

void _DEFUN(Bfree, (ptr, v), struct _reent *ptr _AND _Bigint *v) {
  if (v) {
    v->_next = ptr->_freelist[v->_k];
    ptr->_freelist[v->_k] = v;
  }
}

_Bigint *_DEFUN(multadd, (ptr, b, m, a),
                struct _reent *ptr _AND _Bigint *b _AND int m _AND int a) {
  int i, wds;
  unsigned long *x, y;
>>>>>>> master
#ifdef Pack_32
    unsigned long xi, z;
#endif
    _Bigint* b1;

<<<<<<< HEAD
    wds = b->_wds;
    x   = b->_x;
    i   = 0;
    do
    {
#ifdef Pack_32
        xi   = *x;
        y    = (xi & 0xffff) * m + a;
        z    = (xi >> 16) * m + (y >> 16);
        a    = (int)(z >> 16);
        *x++ = (z << 16) + (y & 0xffff);
#else
        y    = *x * m + a;
        a    = (int)(y >> 16);
        *x++ = y & 0xffff;
#endif
    } while (++i < wds);
    if (a) {
        if (wds >= b->_maxwds) {
            b1 = Balloc(ptr, b->_k + 1);
            Bcopy(b1, b);
            Bfree(ptr, b);
            b = b1;
        }
        b->_x[wds++] = a;
        b->_wds      = wds;
    }
    return b;
}

_Bigint* _DEFUN(
    s2b, (ptr, s, nd0, nd, y9), struct _reent* ptr _AND _CONST char* s _AND int nd0 _AND int nd _AND unsigned long y9)
{
    _Bigint* b;
    int      i, k;
    long     x, y;

    x = (nd + 8) / 9;
    for (k = 0, y = 1; x > y; y <<= 1, k++)
        ;
#ifdef Pack_32
    b        = Balloc(ptr, k);
    b->_x[0] = y9;
    b->_wds  = 1;
#else
    b        = Balloc(ptr, k + 1);
    b->_x[0] = y9 & 0xffff;
    b->_wds = (b->_x[1] = y9 >> 16) ? 2 : 1;
#endif

    i = 9;
    if (9 < nd0) {
        s += 9;
        do
            b = multadd(ptr, b, 10, *s++ - '0');
        while (++i < nd0);
        s++;
    }
    else
        s += 10;
    for (; i < nd; i++) b = multadd(ptr, b, 10, *s++ - '0');
    return b;
}

int _DEFUN(hi0bits, (x), register unsigned long x)
{
    register int k = 0;

    if (!(x & 0xffff0000)) {
        k = 16;
        x <<= 16;
    }
    if (!(x & 0xff000000)) {
        k += 8;
        x <<= 8;
    }
    if (!(x & 0xf0000000)) {
        k += 4;
        x <<= 4;
    }
    if (!(x & 0xc0000000)) {
        k += 2;
        x <<= 2;
    }
    if (!(x & 0x80000000)) {
        k++;
        if (!(x & 0x40000000)) return 32;
    }
    return k;
}

int _DEFUN(lo0bits, (y), unsigned long* y)
{
    register int           k;
    register unsigned long x = *y;

    if (x & 7) {
        if (x & 1) return 0;
        if (x & 2) {
            *y = x >> 1;
            return 1;
        }
        *y = x >> 2;
        return 2;
    }
    k = 0;
    if (!(x & 0xffff)) {
        k = 16;
        x >>= 16;
    }
    if (!(x & 0xff)) {
        k += 8;
        x >>= 8;
    }
    if (!(x & 0xf)) {
        k += 4;
        x >>= 4;
    }
    if (!(x & 0x3)) {
        k += 2;
        x >>= 2;
    }
    if (!(x & 1)) {
        k++;
        x >>= 1;
        if (!x & 1) return 32;
    }
    *y = x;
    return k;
}

_Bigint* _DEFUN(i2b, (ptr, i), struct _reent* ptr _AND int i)
{
    _Bigint* b;

    b        = Balloc(ptr, 1);
    b->_x[0] = i;
    b->_wds  = 1;
    return b;
}

_Bigint* _DEFUN(mult, (ptr, a, b), struct _reent* ptr _AND _Bigint* a _AND _Bigint* b)
{
    _Bigint*       c;
    int            k, wa, wb, wc;
    unsigned long  carry, y, z;
    unsigned long *x, *xa, *xae, *xb, *xbe, *xc, *xc0;
=======
  wds = b->_wds;
  x = b->_x;
  i = 0;
  do {
#ifdef Pack_32
    xi = *x;
    y = (xi & 0xffff) * m + a;
    z = (xi >> 16) * m + (y >> 16);
    a = (int)(z >> 16);
    *x++ = (z << 16) + (y & 0xffff);
#else
    y = *x * m + a;
    a = (int)(y >> 16);
    *x++ = y & 0xffff;
#endif
  } while (++i < wds);
  if (a) {
    if (wds >= b->_maxwds) {
      b1 = Balloc(ptr, b->_k + 1);
      Bcopy(b1, b);
      Bfree(ptr, b);
      b = b1;
    }
    b->_x[wds++] = a;
    b->_wds = wds;
  }
  return b;
}

_Bigint *_DEFUN(s2b, (ptr, s, nd0, nd, y9),
                struct _reent *ptr _AND _CONST char *s _AND int nd0 _AND int nd
                    _AND unsigned long y9) {
  _Bigint *b;
  int i, k;
  long x, y;

  x = (nd + 8) / 9;
  for (k = 0, y = 1; x > y; y <<= 1, k++)
    ;
#ifdef Pack_32
  b = Balloc(ptr, k);
  b->_x[0] = y9;
  b->_wds = 1;
#else
  b = Balloc(ptr, k + 1);
  b->_x[0] = y9 & 0xffff;
  b->_wds = (b->_x[1] = y9 >> 16) ? 2 : 1;
#endif

  i = 9;
  if (9 < nd0) {
    s += 9;
    do
      b = multadd(ptr, b, 10, *s++ - '0');
    while (++i < nd0);
    s++;
  } else
    s += 10;
  for (; i < nd; i++)
    b = multadd(ptr, b, 10, *s++ - '0');
  return b;
}

int _DEFUN(hi0bits, (x), register unsigned long x) {
  register int k = 0;

  if (!(x & 0xffff0000)) {
    k = 16;
    x <<= 16;
  }
  if (!(x & 0xff000000)) {
    k += 8;
    x <<= 8;
  }
  if (!(x & 0xf0000000)) {
    k += 4;
    x <<= 4;
  }
  if (!(x & 0xc0000000)) {
    k += 2;
    x <<= 2;
  }
  if (!(x & 0x80000000)) {
    k++;
    if (!(x & 0x40000000))
      return 32;
  }
  return k;
}

int _DEFUN(lo0bits, (y), unsigned long *y) {
  register int k;
  register unsigned long x = *y;

  if (x & 7) {
    if (x & 1)
      return 0;
    if (x & 2) {
      *y = x >> 1;
      return 1;
    }
    *y = x >> 2;
    return 2;
  }
  k = 0;
  if (!(x & 0xffff)) {
    k = 16;
    x >>= 16;
  }
  if (!(x & 0xff)) {
    k += 8;
    x >>= 8;
  }
  if (!(x & 0xf)) {
    k += 4;
    x >>= 4;
  }
  if (!(x & 0x3)) {
    k += 2;
    x >>= 2;
  }
  if (!(x & 1)) {
    k++;
    x >>= 1;
    if (!x & 1)
      return 32;
  }
  *y = x;
  return k;
}

_Bigint *_DEFUN(i2b, (ptr, i), struct _reent *ptr _AND int i) {
  _Bigint *b;

  b = Balloc(ptr, 1);
  b->_x[0] = i;
  b->_wds = 1;
  return b;
}

_Bigint *_DEFUN(mult, (ptr, a, b),
                struct _reent *ptr _AND _Bigint *a _AND _Bigint *b) {
  _Bigint *c;
  int k, wa, wb, wc;
  unsigned long carry, y, z;
  unsigned long *x, *xa, *xae, *xb, *xbe, *xc, *xc0;
#ifdef Pack_32
  unsigned long z2;
#endif

  if (a->_wds < b->_wds) {
    c = a;
    a = b;
    b = c;
  }
  k = a->_k;
  wa = a->_wds;
  wb = b->_wds;
  wc = wa + wb;
  if (wc > a->_maxwds)
    k++;
  c = Balloc(ptr, k);
  for (x = c->_x, xa = x + wc; x < xa; x++)
    *x = 0;
  xa = a->_x;
  xae = xa + wa;
  xb = b->_x;
  xbe = xb + wb;
  xc0 = c->_x;
#ifdef Pack_32
  for (; xb < xbe; xb++, xc0++) {
    if (y = *xb & 0xffff) {
      x = xa;
      xc = xc0;
      carry = 0;
      do {
        z = (*x & 0xffff) * y + (*xc & 0xffff) + carry;
        carry = z >> 16;
        z2 = (*x++ >> 16) * y + (*xc >> 16) + carry;
        carry = z2 >> 16;
        Storeinc(xc, z2, z);
      } while (x < xae);
      *xc = carry;
    }
    if (y = *xb >> 16) {
      x = xa;
      xc = xc0;
      carry = 0;
      z2 = *xc;
      do {
        z = (*x & 0xffff) * y + (*xc >> 16) + carry;
        carry = z >> 16;
        Storeinc(xc, z, z2);
        z2 = (*x++ >> 16) * y + (*xc & 0xffff) + carry;
        carry = z2 >> 16;
      } while (x < xae);
      *xc = z2;
    }
  }
#else
  for (; xb < xbe; xc0++) {
    if (y = *xb++) {
      x = xa;
      xc = xc0;
      carry = 0;
      do {
        z = *x++ * y + *xc + carry;
        carry = z >> 16;
        *xc++ = z & 0xffff;
      } while (x < xae);
      *xc = carry;
    }
  }
#endif
  for (xc0 = c->_x, xc = xc0 + wc; wc > 0 && !*--xc; --wc)
    ;
  c->_wds = wc;
  return c;
}

_Bigint *_DEFUN(pow5mult, (ptr, b, k),
                struct _reent *ptr _AND _Bigint *b _AND int k) {
  _Bigint *b1, *p5, *p51;
  int i;
  static _CONST int p05[3] = {5, 25, 125};

  if (i = k & 3)
    b = multadd(ptr, b, p05[i - 1], 0);

  if (!(k >>= 2))
    return b;
  if (!(p5 = ptr->_p5s)) {
    /* first time */
    p5 = ptr->_p5s = i2b(ptr, 625);
    p5->_next = 0;
  }
  for (;;) {
    if (k & 1) {
      b1 = mult(ptr, b, p5);
      Bfree(ptr, b);
      b = b1;
    }
    if (!(k >>= 1))
      break;
    if (!(p51 = p5->_next)) {
      p51 = p5->_next = mult(ptr, p5, p5);
      p51->_next = 0;
    }
    p5 = p51;
  }
  return b;
}

_Bigint *_DEFUN(lshift, (ptr, b, k),
                struct _reent *ptr _AND _Bigint *b _AND int k) {
  int i, k1, n, n1;
  _Bigint *b1;
  unsigned long *x, *x1, *xe, z;

#ifdef Pack_32
  n = k >> 5;
#else
  n = k >> 4;
#endif
  k1 = b->_k;
  n1 = n + b->_wds + 1;
  for (i = b->_maxwds; n1 > i; i <<= 1)
    k1++;
  b1 = Balloc(ptr, k1);
  x1 = b1->_x;
  for (i = 0; i < n; i++)
    *x1++ = 0;
  x = b->_x;
  xe = x + b->_wds;
#ifdef Pack_32
  if (k &= 0x1f) {
    k1 = 32 - k;
    z = 0;
    do {
      *x1++ = *x << k | z;
      z = *x++ >> k1;
    } while (x < xe);
    if (*x1 = z)
      ++n1;
  }
#else
  if (k &= 0xf) {
    k1 = 16 - k;
    z = 0;
    do {
      *x1++ = *x << k & 0xffff | z;
      z = *x++ >> k1;
    } while (x < xe);
    if (*x1 = z)
      ++n1;
  }
#endif
  else
    do
      *x1++ = *x++;
    while (x < xe);
  b1->_wds = n1 - 1;
  Bfree(ptr, b);
  return b1;
}

int _DEFUN(cmp, (a, b), _Bigint *a _AND _Bigint *b) {
  unsigned long *xa, *xa0, *xb, *xb0;
  int i, j;

  i = a->_wds;
  j = b->_wds;
#ifdef DEBUG
  if (i > 1 && !a->_x[i - 1])
    Bug("cmp called with a->_x[a->_wds-1] == 0");
  if (j > 1 && !b->_x[j - 1])
    Bug("cmp called with b->_x[b->_wds-1] == 0");
#endif
  if (i -= j)
    return i;
  xa0 = a->_x;
  xa = xa0 + j;
  xb0 = b->_x;
  xb = xb0 + j;
  for (;;) {
    if (*--xa != *--xb)
      return *xa < *xb ? -1 : 1;
    if (xa <= xa0)
      break;
  }
  return 0;
}

_Bigint *_DEFUN(diff, (ptr, a, b),
                struct _reent *ptr _AND _Bigint *a _AND _Bigint *b) {
  _Bigint *c;
  int i, wa, wb;
  long borrow, y; /* We need signed shifts here. */
  unsigned long *xa, *xae, *xb, *xbe, *xc;
>>>>>>> master
#ifdef Pack_32
    unsigned long z2;
#endif

<<<<<<< HEAD
    if (a->_wds < b->_wds) {
        c = a;
        a = b;
        b = c;
    }
    k  = a->_k;
    wa = a->_wds;
    wb = b->_wds;
    wc = wa + wb;
    if (wc > a->_maxwds) k++;
    c = Balloc(ptr, k);
    for (x = c->_x, xa = x + wc; x < xa; x++) *x = 0;
    xa  = a->_x;
    xae = xa + wa;
    xb  = b->_x;
    xbe = xb + wb;
    xc0 = c->_x;
#ifdef Pack_32
    for (; xb < xbe; xb++, xc0++) {
        if (y = *xb & 0xffff) {
            x     = xa;
            xc    = xc0;
            carry = 0;
            do
            {
                z     = (*x & 0xffff) * y + (*xc & 0xffff) + carry;
                carry = z >> 16;
                z2    = (*x++ >> 16) * y + (*xc >> 16) + carry;
                carry = z2 >> 16;
                Storeinc(xc, z2, z);
            } while (x < xae);
            *xc = carry;
        }
        if (y = *xb >> 16) {
            x     = xa;
            xc    = xc0;
            carry = 0;
            z2    = *xc;
            do
            {
                z     = (*x & 0xffff) * y + (*xc >> 16) + carry;
                carry = z >> 16;
                Storeinc(xc, z, z2);
                z2    = (*x++ >> 16) * y + (*xc & 0xffff) + carry;
                carry = z2 >> 16;
            } while (x < xae);
            *xc = z2;
        }
    }
#else
    for (; xb < xbe; xc0++) {
        if (y = *xb++) {
            x     = xa;
            xc    = xc0;
            carry = 0;
            do
            {
                z     = *x++ * y + *xc + carry;
                carry = z >> 16;
                *xc++ = z & 0xffff;
            } while (x < xae);
            *xc = carry;
        }
    }
=======
  i = cmp(a, b);
  if (!i) {
    c = Balloc(ptr, 0);
    c->_wds = 1;
    c->_x[0] = 0;
    return c;
  }
  if (i < 0) {
    c = a;
    a = b;
    b = c;
    i = 1;
  } else
    i = 0;
  c = Balloc(ptr, a->_k);
  c->_sign = i;
  wa = a->_wds;
  xa = a->_x;
  xae = xa + wa;
  wb = b->_wds;
  xb = b->_x;
  xbe = xb + wb;
  xc = c->_x;
  borrow = 0;
#ifdef Pack_32
  do {
    y = (*xa & 0xffff) - (*xb & 0xffff) + borrow;
    borrow = y >> 16;
    Sign_Extend(borrow, y);
    z = (*xa++ >> 16) - (*xb++ >> 16) + borrow;
    borrow = z >> 16;
    Sign_Extend(borrow, z);
    Storeinc(xc, z, y);
  } while (xb < xbe);
  while (xa < xae) {
    y = (*xa & 0xffff) + borrow;
    borrow = y >> 16;
    Sign_Extend(borrow, y);
    z = (*xa++ >> 16) + borrow;
    borrow = z >> 16;
    Sign_Extend(borrow, z);
    Storeinc(xc, z, y);
  }
#else
  do {
    y = *xa++ - *xb++ + borrow;
    borrow = y >> 16;
    Sign_Extend(borrow, y);
    *xc++ = y & 0xffff;
  } while (xb < xbe);
  while (xa < xae) {
    y = *xa++ + borrow;
    borrow = y >> 16;
    Sign_Extend(borrow, y);
    *xc++ = y & 0xffff;
  }
>>>>>>> master
#endif
    for (xc0 = c->_x, xc = xc0 + wc; wc > 0 && !*--xc; --wc)
        ;
    c->_wds = wc;
    return c;
}

<<<<<<< HEAD
_Bigint* _DEFUN(pow5mult, (ptr, b, k), struct _reent* ptr _AND _Bigint* b _AND int k)
{
    _Bigint *         b1, *p5, *p51;
    int               i;
    static _CONST int p05[3] = {5, 25, 125};

    if (i = k & 3) b = multadd(ptr, b, p05[i - 1], 0);

    if (!(k >>= 2)) return b;
    if (!(p5 = ptr->_p5s)) {
        /* first time */
        p5 = ptr->_p5s = i2b(ptr, 625);
        p5->_next      = 0;
    }
    for (;;) {
        if (k & 1) {
            b1 = mult(ptr, b, p5);
            Bfree(ptr, b);
            b = b1;
        }
        if (!(k >>= 1)) break;
        if (!(p51 = p5->_next)) {
            p51 = p5->_next = mult(ptr, p5, p5);
            p51->_next      = 0;
        }
        p5 = p51;
    }
    return b;
}

_Bigint* _DEFUN(lshift, (ptr, b, k), struct _reent* ptr _AND _Bigint* b _AND int k)
{
    int            i, k1, n, n1;
    _Bigint*       b1;
    unsigned long *x, *x1, *xe, z;

#ifdef Pack_32
    n = k >> 5;
#else
    n = k >> 4;
#endif
    k1 = b->_k;
    n1 = n + b->_wds + 1;
    for (i = b->_maxwds; n1 > i; i <<= 1) k1++;
    b1 = Balloc(ptr, k1);
    x1 = b1->_x;
    for (i = 0; i < n; i++) *x1++ = 0;
    x                             = b->_x;
    xe                            = x + b->_wds;
#ifdef Pack_32
    if (k &= 0x1f) {
        k1 = 32 - k;
        z  = 0;
        do
        {
            *x1++ = *x << k | z;
            z     = *x++ >> k1;
        } while (x < xe);
        if (*x1 = z) ++n1;
    }
#else
    if (k &= 0xf) {
        k1 = 16 - k;
        z  = 0;
        do
        {
            *x1++ = *x << k & 0xffff | z;
            z     = *x++ >> k1;
        } while (x < xe);
        if (*x1 = z) ++n1;
    }
#endif
    else
        do
            *x1++ = *x++;
        while (x < xe);
    b1->_wds = n1 - 1;
    Bfree(ptr, b);
    return b1;
}

int _DEFUN(cmp, (a, b), _Bigint* a _AND _Bigint* b)
{
    unsigned long *xa, *xa0, *xb, *xb0;
    int            i, j;

    i = a->_wds;
    j = b->_wds;
#ifdef DEBUG
    if (i > 1 && !a->_x[i - 1]) Bug("cmp called with a->_x[a->_wds-1] == 0");
    if (j > 1 && !b->_x[j - 1]) Bug("cmp called with b->_x[b->_wds-1] == 0");
#endif
    if (i -= j) return i;
    xa0 = a->_x;
    xa  = xa0 + j;
    xb0 = b->_x;
    xb  = xb0 + j;
    for (;;) {
        if (*--xa != *--xb) return *xa < *xb ? -1 : 1;
        if (xa <= xa0) break;
    }
    return 0;
}

_Bigint* _DEFUN(diff, (ptr, a, b), struct _reent* ptr _AND _Bigint* a _AND _Bigint* b)
{
    _Bigint*       c;
    int            i, wa, wb;
    long           borrow, y; /* We need signed shifts here. */
    unsigned long *xa, *xae, *xb, *xbe, *xc;
#ifdef Pack_32
    long z;
#endif

    i = cmp(a, b);
    if (!i) {
        c        = Balloc(ptr, 0);
        c->_wds  = 1;
        c->_x[0] = 0;
        return c;
    }
    if (i < 0) {
        c = a;
        a = b;
        b = c;
        i = 1;
    }
    else
        i    = 0;
    c        = Balloc(ptr, a->_k);
    c->_sign = i;
    wa       = a->_wds;
    xa       = a->_x;
    xae      = xa + wa;
    wb       = b->_wds;
    xb       = b->_x;
    xbe      = xb + wb;
    xc       = c->_x;
    borrow   = 0;
#ifdef Pack_32
    do
    {
        y      = (*xa & 0xffff) - (*xb & 0xffff) + borrow;
        borrow = y >> 16;
        Sign_Extend(borrow, y);
        z      = (*xa++ >> 16) - (*xb++ >> 16) + borrow;
        borrow = z >> 16;
        Sign_Extend(borrow, z);
        Storeinc(xc, z, y);
    } while (xb < xbe);
    while (xa < xae) {
        y      = (*xa & 0xffff) + borrow;
        borrow = y >> 16;
        Sign_Extend(borrow, y);
        z      = (*xa++ >> 16) + borrow;
        borrow = z >> 16;
        Sign_Extend(borrow, z);
        Storeinc(xc, z, y);
    }
#else
    do
    {
        y      = *xa++ - *xb++ + borrow;
        borrow = y >> 16;
        Sign_Extend(borrow, y);
        *xc++ = y & 0xffff;
    } while (xb < xbe);
    while (xa < xae) {
        y      = *xa++ + borrow;
        borrow = y >> 16;
        Sign_Extend(borrow, y);
        *xc++ = y & 0xffff;
    }
#endif
    while (!*--xc) wa--;
    c->_wds = wa;
    return c;
}

double _DEFUN(ulp, (x), double x)
{
    register long L;
    double        a;

    L = (word0(x) & Exp_mask) - (P - 1) * Exp_msk1;
#ifndef Sudden_Underflow
    if (L > 0) {
#endif
#ifdef IBM
        L |= Exp_msk1 >> 4;
#endif
        word0(a) = L;
        word1(a) = 0;
=======
double _DEFUN(ulp, (x), double x) {
  register long L;
  double a;

  L = (word0(x) & Exp_mask) - (P - 1) * Exp_msk1;
#ifndef Sudden_Underflow
  if (L > 0) {
#endif
#ifdef IBM
    L |= Exp_msk1 >> 4;
#endif
    word0(a) = L;
    word1(a) = 0;
>>>>>>> master
#ifndef Sudden_Underflow
  } else {
    L = -L >> Exp_shift;
    if (L < Exp_shift) {
      word0(a) = 0x80000 >> L;
      word1(a) = 0;
    } else {
      word0(a) = 0;
      L -= Exp_shift;
      word1(a) = L >= 31 ? 1 : 1 << 31 - L;
    }
<<<<<<< HEAD
    else
    {
        L = -L >> Exp_shift;
        if (L < Exp_shift) {
            word0(a) = 0x80000 >> L;
            word1(a) = 0;
        }
        else
        {
            word0(a) = 0;
            L -= Exp_shift;
            word1(a) = L >= 31 ? 1 : 1 << 31 - L;
        }
    }
=======
  }
>>>>>>> master
#endif
    return a;
}

<<<<<<< HEAD
double _DEFUN(b2d, (a, e), _Bigint* a _AND int* e)
{
    unsigned long *xa, *xa0, w, y, z;
    int            k;
    double         d;
=======
double _DEFUN(b2d, (a, e), _Bigint *a _AND int *e) {
  unsigned long *xa, *xa0, w, y, z;
  int k;
  double d;
>>>>>>> master
#ifdef VAX
    unsigned long d0, d1;
#else
#define d0 word0(d)
#define d1 word1(d)
#endif

    xa0 = a->_x;
    xa  = xa0 + a->_wds;
    y   = *--xa;
#ifdef DEBUG
<<<<<<< HEAD
    if (!y) Bug("zero y in b2d");
#endif
    k  = hi0bits(y);
    *e = 32 - k;
#ifdef Pack_32
    if (k < Ebits) {
        d0 = Exp_1 | y >> Ebits - k;
        w  = xa > xa0 ? *--xa : 0;
        d1 = y << (32 - Ebits) + k | w >> Ebits - k;
        goto ret_d;
    }
    z = xa > xa0 ? *--xa : 0;
    if (k -= Ebits) {
        d0 = Exp_1 | y << k | z >> 32 - k;
        y  = xa > xa0 ? *--xa : 0;
        d1 = z << k | y >> 32 - k;
    }
    else
    {
        d0 = Exp_1 | y;
        d1 = z;
    }
#else
    if (k < Ebits + 16) {
        z  = xa > xa0 ? *--xa : 0;
        d0 = Exp_1 | y << k - Ebits | z >> Ebits + 16 - k;
        w  = xa > xa0 ? *--xa : 0;
        y  = xa > xa0 ? *--xa : 0;
        d1 = z << k + 16 - Ebits | w << k - Ebits | y >> 16 + Ebits - k;
        goto ret_d;
    }
    z = xa > xa0 ? *--xa : 0;
    w = xa > xa0 ? *--xa : 0;
    k -= Ebits + 16;
    d0 = Exp_1 | y << k + 16 | z << k | w >> 16 - k;
    y  = xa > xa0 ? *--xa : 0;
    d1 = w << k + 16 | y << k;
#endif
ret_d:
#ifdef VAX
    word0(d) = d0 >> 16 | d0 << 16;
    word1(d) = d1 >> 16 | d1 << 16;
=======
  if (!y)
    Bug("zero y in b2d");
#endif
  k = hi0bits(y);
  *e = 32 - k;
#ifdef Pack_32
  if (k < Ebits) {
    d0 = Exp_1 | y >> Ebits - k;
    w = xa > xa0 ? *--xa : 0;
    d1 = y << (32 - Ebits) + k | w >> Ebits - k;
    goto ret_d;
  }
  z = xa > xa0 ? *--xa : 0;
  if (k -= Ebits) {
    d0 = Exp_1 | y << k | z >> 32 - k;
    y = xa > xa0 ? *--xa : 0;
    d1 = z << k | y >> 32 - k;
  } else {
    d0 = Exp_1 | y;
    d1 = z;
  }
#else
  if (k < Ebits + 16) {
    z = xa > xa0 ? *--xa : 0;
    d0 = Exp_1 | y << k - Ebits | z >> Ebits + 16 - k;
    w = xa > xa0 ? *--xa : 0;
    y = xa > xa0 ? *--xa : 0;
    d1 = z << k + 16 - Ebits | w << k - Ebits | y >> 16 + Ebits - k;
    goto ret_d;
  }
  z = xa > xa0 ? *--xa : 0;
  w = xa > xa0 ? *--xa : 0;
  k -= Ebits + 16;
  d0 = Exp_1 | y << k + 16 | z << k | w >> 16 - k;
  y = xa > xa0 ? *--xa : 0;
  d1 = w << k + 16 | y << k;
#endif
ret_d:
#ifdef VAX
  word0(d) = d0 >> 16 | d0 << 16;
  word1(d) = d1 >> 16 | d1 << 16;
>>>>>>> master
#else
#undef d0
#undef d1
#endif
    return d;
}

<<<<<<< HEAD
_Bigint* _DEFUN(d2b, (ptr, d, e, bits), struct _reent* ptr _AND double d _AND int* e _AND int* bits)
=======
_Bigint *_DEFUN(d2b, (ptr, d, e, bits),
                struct _reent *ptr _AND double d _AND int *e _AND int *bits)
>>>>>>> master

{
    _Bigint*       b;
    int            de, i, k;
    unsigned long *x, y, z;
#ifdef VAX
<<<<<<< HEAD
    unsigned long d0, d1;
    d0 = word0(d) >> 16 | word0(d) << 16;
    d1 = word1(d) >> 16 | word1(d) << 16;
=======
  unsigned long d0, d1;
  d0 = word0(d) >> 16 | word0(d) << 16;
  d1 = word1(d) >> 16 | word1(d) << 16;
>>>>>>> master
#else
#define d0 word0(d)
#define d1 word1(d)
#endif

#ifdef Pack_32
<<<<<<< HEAD
    b = Balloc(ptr, 1);
#else
    b  = Balloc(ptr, 2);
=======
  b = Balloc(ptr, 1);
#else
  b = Balloc(ptr, 2);
>>>>>>> master
#endif
    x = b->_x;

<<<<<<< HEAD
    z = d0 & Frac_mask;
    d0 &= 0x7fffffff; /* clear sign bit, which we ignore */
#ifdef Sudden_Underflow
    de = (int)(d0 >> Exp_shift);
=======
  z = d0 & Frac_mask;
  d0 &= 0x7fffffff; /* clear sign bit, which we ignore */
#ifdef Sudden_Underflow
  de = (int)(d0 >> Exp_shift);
>>>>>>> master
#ifndef IBM
    z |= Exp_msk11;
#endif
#else
<<<<<<< HEAD
    if (de = (int)(d0 >> Exp_shift)) z |= Exp_msk1;
#endif
#ifdef Pack_32
    if (y = d1) {
        if (k = lo0bits(&y)) {
            x[0] = y | z << 32 - k;
            z >>= k;
        }
        else
            x[0] = y;
        i = b->_wds = (x[1] = z) ? 2 : 1;
    }
    else
    {
#ifdef DEBUG
        if (!z) Bug("Zero passed to d2b");
#endif
        k    = lo0bits(&z);
        x[0] = z;
        i = b->_wds = 1;
        k += 32;
    }
#else
    if (y = d1) {
        if (k = lo0bits(&y))
            if (k >= 16) {
                x[0] = y | z << 32 - k & 0xffff;
                x[1] = z >> k - 16 & 0xffff;
                x[2] = z >> k;
                i    = 2;
            }
            else
            {
                x[0] = y & 0xffff;
                x[1] = y >> 16 | z << 16 - k & 0xffff;
                x[2] = z >> k & 0xffff;
                x[3] = z >> k + 16;
                i    = 3;
            }
        else
        {
            x[0] = y & 0xffff;
            x[1] = y >> 16;
            x[2] = z & 0xffff;
            x[3] = z >> 16;
            i    = 3;
        }
    }
    else
    {
#ifdef DEBUG
        if (!z) Bug("Zero passed to d2b");
#endif
        k = lo0bits(&z);
        if (k >= 16) {
            x[0] = z;
            i    = 0;
        }
        else
        {
            x[0] = z & 0xffff;
            x[1] = z >> 16;
            i    = 1;
        }
        k += 32;
    }
    while (!x[i]) --i;
    b->_wds = i + 1;
#endif
#ifndef Sudden_Underflow
    if (de) {
#endif
#ifdef IBM
        *e    = (de - Bias - (P - 1) << 2) + k;
        *bits = 4 * P + 8 - k - hi0bits(word0(d) & Frac_mask);
#else
    *e      = de - Bias - (P - 1) + k;
    *bits   = P - k;
#endif
#ifndef Sudden_Underflow
    }
    else
    {
        *e = de - Bias - (P - 1) + 1 + k;
#ifdef Pack_32
        *bits = 32 * i - hi0bits(x[i - 1]);
#else
        *bits = (i + 2) * 16 - hi0bits(x[i]);
=======
  if (de = (int)(d0 >> Exp_shift))
    z |= Exp_msk1;
#endif
#ifdef Pack_32
  if (y = d1) {
    if (k = lo0bits(&y)) {
      x[0] = y | z << 32 - k;
      z >>= k;
    } else
      x[0] = y;
    i = b->_wds = (x[1] = z) ? 2 : 1;
  } else {
#ifdef DEBUG
    if (!z)
      Bug("Zero passed to d2b");
#endif
    k = lo0bits(&z);
    x[0] = z;
    i = b->_wds = 1;
    k += 32;
  }
#else
  if (y = d1) {
    if (k = lo0bits(&y))
      if (k >= 16) {
        x[0] = y | z << 32 - k & 0xffff;
        x[1] = z >> k - 16 & 0xffff;
        x[2] = z >> k;
        i = 2;
      } else {
        x[0] = y & 0xffff;
        x[1] = y >> 16 | z << 16 - k & 0xffff;
        x[2] = z >> k & 0xffff;
        x[3] = z >> k + 16;
        i = 3;
      }
    else {
      x[0] = y & 0xffff;
      x[1] = y >> 16;
      x[2] = z & 0xffff;
      x[3] = z >> 16;
      i = 3;
    }
  } else {
#ifdef DEBUG
    if (!z)
      Bug("Zero passed to d2b");
#endif
    k = lo0bits(&z);
    if (k >= 16) {
      x[0] = z;
      i = 0;
    } else {
      x[0] = z & 0xffff;
      x[1] = z >> 16;
      i = 1;
    }
    k += 32;
  }
  while (!x[i])
    --i;
  b->_wds = i + 1;
#endif
#ifndef Sudden_Underflow
  if (de) {
#endif
#ifdef IBM
    *e = (de - Bias - (P - 1) << 2) + k;
    *bits = 4 * P + 8 - k - hi0bits(word0(d) & Frac_mask);
#else
  *e = de - Bias - (P - 1) + k;
  *bits = P - k;
#endif
#ifndef Sudden_Underflow
  } else {
    *e = de - Bias - (P - 1) + 1 + k;
#ifdef Pack_32
    *bits = 32 * i - hi0bits(x[i - 1]);
#else
    *bits = (i + 2) * 16 - hi0bits(x[i]);
>>>>>>> master
#endif
  }
#endif
    return b;
}
#undef d0
#undef d1

<<<<<<< HEAD
double _DEFUN(ratio, (a, b), _Bigint* a _AND _Bigint* b)
=======
double _DEFUN(ratio, (a, b), _Bigint *a _AND _Bigint *b)
>>>>>>> master

{
    double da, db;
    int    k, ka, kb;

<<<<<<< HEAD
    da = b2d(a, &ka);
    db = b2d(b, &kb);
=======
  da = b2d(a, &ka);
  db = b2d(b, &kb);
>>>>>>> master
#ifdef Pack_32
    k = ka - kb + 32 * (a->_wds - b->_wds);
#else
    k       = ka - kb + 16 * (a->_wds - b->_wds);
#endif
#ifdef IBM
<<<<<<< HEAD
    if (k > 0) {
        word0(da) += (k >> 2) * Exp_msk1;
        if (k &= 3) da *= 1 << k;
    }
    else
    {
        k = -k;
        word0(db) += (k >> 2) * Exp_msk1;
        if (k &= 3) db *= 1 << k;
    }
#else
    if (k > 0)
        word0(da) += k * Exp_msk1;
    else
    {
        k = -k;
        word0(db) += k * Exp_msk1;
    }
=======
  if (k > 0) {
    word0(da) += (k >> 2) * Exp_msk1;
    if (k &= 3)
      da *= 1 << k;
  } else {
    k = -k;
    word0(db) += (k >> 2) * Exp_msk1;
    if (k &= 3)
      db *= 1 << k;
  }
#else
  if (k > 0)
    word0(da) += k * Exp_msk1;
  else {
    k = -k;
    word0(db) += k * Exp_msk1;
  }
>>>>>>> master
#endif
    return da / db;
}

<<<<<<< HEAD
_CONST double tens[] = {1e0,
    1e1,
    1e2,
    1e3,
    1e4,
    1e5,
    1e6,
    1e7,
    1e8,
    1e9,
    1e10,
    1e11,
    1e12,
    1e13,
    1e14,
    1e15,
    1e16,
    1e17,
    1e18,
    1e19,
    1e20,
    1e21,
    1e22,
    1e23,
    1e24
=======
_CONST double tens[] = {1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,
                        1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17,
                        1e18, 1e19, 1e20, 1e21, 1e22, 1e23, 1e24
>>>>>>> master

};

#if !defined(_DOUBLE_IS_32BITS) && !defined(__v800)
_CONST double bigtens[] = {1e16, 1e32, 1e64, 1e128, 1e256};

_CONST double tinytens[] = {1e-16, 1e-32, 1e-64, 1e-128, 1e-256};
#else
_CONST double bigtens[] = {1e16, 1e32};

_CONST double tinytens[] = {1e-16, 1e-32};
#endif

<<<<<<< HEAD
double _DEFUN(_mprec_log10, (dig), int dig)
{
    double v = 1.0;
    if (dig < 24) return tens[dig];
    while (dig > 0) {
        v *= 10;
        dig--;
    }
    return v;
=======
double _DEFUN(_mprec_log10, (dig), int dig) {
  double v = 1.0;
  if (dig < 24)
    return tens[dig];
  while (dig > 0) {
    v *= 10;
    dig--;
  }
  return v;
>>>>>>> master
}
