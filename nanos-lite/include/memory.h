#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#ifndef PGSIZE
#define PGSIZE 4096
#endif

#define PG_ALIGN __attribute((aligned(PGSIZE)))

<<<<<<< HEAD
#define PGMASK (PGSIZE - 1)  // Mask for bit ops
=======
#define PGMASK (PGSIZE - 1) // Mask for bit ops
>>>>>>> master
#define PGROUNDUP(sz) (((sz) + PGSIZE - 1) & ~PGMASK)
#define PGROUNDDOWN(a) (((a)) & ~PGMASK)

void *new_page(void);

#endif
