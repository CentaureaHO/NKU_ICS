/* VxWorks provides its own version of malloc, and we can't use this
   one because VxWorks does not provide sbrk.  So we have a hook to
   not compile this code.  */

#ifdef MALLOC_PROVIDED

int _dummy_mstats = 1;

#else

#include "malloc.h"
#include <_ansi.h>
#include <reent.h>
#include <stdlib.h>

#ifdef MSTATS
#ifndef _REENT_ONLY

<<<<<<< HEAD
void _DEFUN(mstats, (s), char* s) { _mstats_r(_REENT, s); }
=======
void _DEFUN(mstats, (s), char *s) { _mstats_r(_REENT, s); }
>>>>>>> master

#endif
#else

<<<<<<< HEAD
mstats() { /* dummy to keep people happy */}
=======
mstats() { /* dummy to keep people happy */
}
>>>>>>> master

#endif

#endif /* ! defined (MALLOC_PROVIDED) */
