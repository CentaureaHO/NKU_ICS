/*
** $Id: lapi.h,v 2.9 2015/03/06 19:49:50 roberto Exp $
** Auxiliary functions from Lua API
** See Copyright Notice in lua.h
*/

#ifndef lapi_h
#define lapi_h

#include "llimits.h"
#include "lstate.h"

<<<<<<< HEAD
#define api_incr_top(L)                                       \
    {                                                         \
        L->top++;                                             \
        api_check(L, L->top <= L->ci->top, "stack overflow"); \
    }

#define adjustresults(L, nres)                                                 \
    {                                                                          \
        if ((nres) == LUA_MULTRET && L->ci->top < L->top) L->ci->top = L->top; \
    }

#define api_checknelems(L, n)         \
    api_check(L,                      \
        (n) < (L->top - L->ci->func), \
        "not enough elements in the " \
        "stack")
=======
#define api_incr_top(L)                                                        \
  {                                                                            \
    L->top++;                                                                  \
    api_check(L, L->top <= L->ci->top, "stack overflow");                      \
  }

#define adjustresults(L, nres)                                                 \
  {                                                                            \
    if ((nres) == LUA_MULTRET && L->ci->top < L->top)                          \
      L->ci->top = L->top;                                                     \
  }

#define api_checknelems(L, n)                                                  \
  api_check(L, (n) < (L->top - L->ci->func), "not enough elements in the "     \
                                             "stack")
>>>>>>> master

#endif
