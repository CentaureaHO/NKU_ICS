#include "memory.h"
#include "proc.h"

static void* pf = NULL;

void* new_page(void)
{
    assert(pf < (void*)_heap.end);
    void* p = pf;
    pf += PGSIZE;
    return p;
}

void free_page(void* p) { panic("not implement yet"); }

void* alloc_page(size_t n)
{
    assert(pf + (n - 1) * PGSIZE < (void*)_heap.end);
    void* p = pf;
    pf += n * PGSIZE;
    return p;
}

/* The brk() system call handler. */
int mm_brk(uint32_t new_brk)
{
    if (current->cur_brk == 0) {
        current->cur_brk = current->max_brk = new_brk;
        return 0;
    }

    if (new_brk > current->max_brk) {
        uint32_t map_start_va = PGROUNDUP(current->max_brk);
        uint32_t map_end_va   = PGROUNDDOWN(new_brk);
        uint32_t nums_to_map  = 0;

        if (map_start_va != 0 && map_start_va <= map_end_va) nums_to_map = (map_end_va - map_start_va) / PGSIZE + 1;
        if (nums_to_map > 0) {
            void* pa = alloc_page(nums_to_map);

            for (uint32_t va_to_map = map_start_va; va_to_map <= map_end_va; va_to_map += PGSIZE) {
                _map(&current->as, (void*)va_to_map, pa);
                pa += PGSIZE;
            }
        }
        current->max_brk = new_brk;
    }

    current->cur_brk = new_brk;
    return 0;
}

void init_mm()
{
    pf = (void*)PGROUNDUP((uintptr_t)_heap.start);
    Log("free physical pages starting from %p", pf);

    _pte_init(new_page, free_page);
}
