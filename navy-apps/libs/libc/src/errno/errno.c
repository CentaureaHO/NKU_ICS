/* The errno variable is stored in the reentrancy structure.  This
   function returns its address for use by the macro errno defined in
   errno.h.  */

#include <errno.h>
#include <reent.h>

#ifndef _REENT_ONLY

<<<<<<< HEAD
int* __errno() { return &_REENT->_errno; }
=======
int *__errno() { return &_REENT->_errno; }
>>>>>>> master

#endif
