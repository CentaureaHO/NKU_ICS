#include "memory.h"
#include "proc.h"

static void* pf = NULL;

<<<<<<< HEAD
void* new_page(void)
{
    assert(pf < (void*)_heap.end);
    void* p = pf;
    pf += PGSIZE;
    return p;
}

void free_page(void* p) { panic("not implement yet"); }
=======
void *new_page(void) {
  assert(pf < (void *)_heap.end);
  void *p = pf;
  pf += PGSIZE;
  return p;
}

void free_page(void *p) { panic("not implement yet"); }
>>>>>>> master

/* The brk() system call handler. */
int mm_brk(uint32_t new_brk) { return 0; }

void init_mm()
{
    pf = (void*)PGROUNDUP((uintptr_t)_heap.start);
    Log("free physical pages starting from %p", pf);

    _pte_init(new_page, free_page);
}
