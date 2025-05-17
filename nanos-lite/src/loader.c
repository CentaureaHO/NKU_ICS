#include "common.h"
#include "fs.h"

#define DEFAULT_ENTRY ((void*)0x8048000)

extern void ramdisk_read(void* buf, off_t offset, size_t len);
extern void ramdisk_write(const void* buf, off_t offset, size_t len);
extern size_t get_ramdisk_size();
extern void*  new_page(void);
extern void*  alloc_page(size_t n);

uintptr_t loader(_Protect* as, const char* filename)
{
    int    fd   = fs_open(filename, 0, 0);
    size_t size = fs_filesz(fd);

    size_t pages = size / PGSIZE;
    size_t remainder = size % PGSIZE;
    size_t alloc_pages = pages + (remainder ? 1 : 0);
    Log("size = %d, pages = %d, remainder = %d", size, pages, remainder);
    Log("Alloca %d pages", alloc_pages);
    assert(alloc_pages > 0);
    void* paddr = alloc_page(alloc_pages);
    void* vaddr = DEFAULT_ENTRY;

    for (size_t i = 0; i < pages; ++i)
    {
        _map(as, vaddr, paddr);
        fs_read(fd, paddr, PGSIZE);
        paddr += PGSIZE;
        vaddr += PGSIZE;
    }

    if (remainder)
    {
        _map(as, vaddr, paddr);
        fs_read(fd, paddr, remainder);
    }

    fs_close(fd);
    return (uintptr_t)DEFAULT_ENTRY;
}
