/*
 * return (*acc) scaled by 10**dexp.
 */

#include "std.h"
#include <_ansi.h>
#include <reent.h>

#define abs(x) (((x) < 0) ? -(x) : (x))

<<<<<<< HEAD
double _DEFUN(__adjust, (ptr, acc, dexp, sign), struct _reent* ptr _AND double* acc _AND int dexp _AND int sign)
=======
double _DEFUN(__adjust, (ptr, acc, dexp, sign),
              struct _reent *ptr _AND double *acc _AND int dexp _AND int sign)
>>>>>>> master
/* *acc	the 64 bit accumulator */
/* dexp	decimal exponent       */
/* sign	sign flag   	       */
{
    double r;

<<<<<<< HEAD
    if (dexp > MAXE) {
        ptr->_errno = ERANGE;
        return (sign) ? -HUGE_VAL : HUGE_VAL;
    }
    else if (dexp < MINE)
    {
        ptr->_errno = ERANGE;
        return 0.0;
    }
=======
  if (dexp > MAXE) {
    ptr->_errno = ERANGE;
    return (sign) ? -HUGE_VAL : HUGE_VAL;
  } else if (dexp < MINE) {
    ptr->_errno = ERANGE;
    return 0.0;
  }
>>>>>>> master

    r           = *acc;
    if (sign) r = -r;
    if (dexp == 0) return r;

<<<<<<< HEAD
    if (dexp < 0)
        return r / __exp10(abs(dexp));
    else
        return r * __exp10(dexp);
=======
  if (dexp < 0)
    return r / __exp10(abs(dexp));
  else
    return r * __exp10(dexp);
>>>>>>> master
}
