/* Provide support for both ANSI and non-ANSI environments.  */

/* Some ANSI environments are "broken" in the sense that __STDC__ cannot be
   relied upon to have it's intended meaning.  Therefore we must use our own
   concoction: _HAVE_STDC.  Always use _HAVE_STDC instead of __STDC__ in newlib
   sources!

   To get a strict ANSI C environment, define macro _STRICT_ANSI.  This will
   "comment out" the non-ANSI parts of the ANSI header files (non-ANSI header
   files aren't affected).  */

#ifndef _ANSIDECL_H_
#define _ANSIDECL_H_

#include <sys/config.h>

/* First try to figure out whether we really are in an ANSI C environment.  */
/* FIXME: This probably needs some work.  Perhaps sys/config.h can be
   prevailed upon to give us a clue.  */

#if __STDC__ == 1
#define _HAVE_STDC
#endif

#ifdef _HAVE_STDC
<<<<<<< HEAD
#define _PTR void*
=======
#define _PTR void *
>>>>>>> master
#define _AND ,
#define _NOARGS void
#define _CONST const
#define _VOLATILE volatile
#define _SIGNED signed
#define _DOTS , ...
#define _VOID void
#define _EXFUN(name, proto) name proto
#define _DEFUN(name, arglist, args) name(args)
#define _DEFUN_VOID(name) name(_NOARGS)
#define _CAST_VOID (void)
#ifndef _LONG_DOUBLE
#define _LONG_DOUBLE long double
#endif
#ifndef _PARAMS
#define _PARAMS(paramlist) paramlist
#endif
#else
<<<<<<< HEAD
#define _PTR char*
=======
#define _PTR char *
>>>>>>> master
#define _AND ;
#define _NOARGS
#define _CONST
#define _VOLATILE
#define _SIGNED
#define _DOTS
#define _VOID void
#define _EXFUN(name, proto) name()
#define _DEFUN(name, arglist, args) name arglist args;
#define _DEFUN_VOID(name) name()
#define _CAST_VOID
#define _LONG_DOUBLE double
#ifndef _PARAMS
#define _PARAMS(paramlist) ()
#endif
#endif

/* Support gcc's __attribute__ facility.  */

#ifdef __GNUC__
#define _ATTRIBUTE(attrs) __attribute__(attrs)
#else
#define _ATTRIBUTE(attrs)
#endif

#endif /* _ANSIDECL_H_ */
