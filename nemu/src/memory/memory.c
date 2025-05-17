#include "nemu.h"
#include "device/mmio.h"
#include "memory/mmu.h"

#define pmem_rw(addr, type)                                                         \
    *(type*)({                                                                      \
        Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
        guest_to_host(addr);                                                        \
    })

uint8_t pmem[PMEM_SIZE];

paddr_t page_translate(vaddr_t vaddr, bool is_write)
{
    if (!cpu.PG) return vaddr;

    // Log("vaddr = 0x%x, cpu.PDBR = 0x%x", vaddr, cpu.PDBR);

    uint32_t dir_index  = (vaddr >> 22) & 0x3FF;
    uint32_t page_index = (vaddr >> 12) & 0x3FF;
    uint32_t offset     = vaddr & 0xFFF;

    uint32_t pde_base = cpu.PDBR << 12;
    // Log("pde_base = 0x%x", pde_base);
    // Assert(pde_base == 0x1d6e000, "pde_base = 0x%x", pde_base);
    PDE pde;
    pde.val = paddr_read(pde_base + dir_index * sizeof(PDE), sizeof(PDE));

    if (!pde.present)
        Assert(0,
            "Page Directory Entry not present, vaddr = 0x%x, dir_index = %d, addr = 0x%x",
            vaddr,
            dir_index,
            pde_base + dir_index * sizeof(PDE));

    if (pde.accessed == 0) {
        pde.accessed = 1;
        paddr_write(pde_base + dir_index * sizeof(PDE), sizeof(PDE), pde.val);
    }

    uint32_t pte_base = pde.page_frame << 12;

    PTE pte;
    pte.val = paddr_read(pte_base + page_index * sizeof(PTE), sizeof(PTE));

    if (!pte.present)
        Assert(0,
            "Page Table Entry not present, vaddr = 0x%x, dir_index = %d, page_index = %d",
            vaddr,
            dir_index,
            page_index);

    if (pte.accessed == 0 || (is_write && pte.dirty == 0)) {
        if (pte.accessed == 0) pte.accessed = 1;
        if (is_write) pte.dirty             = 1;
        paddr_write(pte_base + page_index * sizeof(PTE), sizeof(PTE), pte.val);
    }

    return (pte.page_frame << 12) | offset;
}

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len)
{
    int mmio_id = is_mmio(addr);

    if (mmio_id == -1) return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));

    return mmio_read(addr, len, mmio_id);
}

void paddr_write(paddr_t addr, int len, uint32_t data)
{
    int mmio_id = is_mmio(addr);

    if (mmio_id == -1) {
        memcpy(guest_to_host(addr), &data, len);
        return;
    }

    mmio_write(addr, len, data, mmio_id);
}

uint32_t vaddr_read(vaddr_t addr, int len)
{
    if (((addr & PAGE_MASK) + len) > PAGE_SIZE) {
        // uint32_t至多读四字节，此次直接认定至多跨一页
        int len1 = PAGE_SIZE - (addr & PAGE_MASK);
        int len2 = len - len1;

        uint32_t data1 = 0;
        uint32_t data2 = 0;

        paddr_t paddr1 = page_translate(addr, false);
        data1          = paddr_read(paddr1, len1);

        paddr_t paddr2 = page_translate(addr + len1, false);
        data2          = paddr_read(paddr2, len2);

        uint32_t result = (data2 << (len1 * 8)) | data1;
        return result;
    }
    else
    {
        paddr_t paddr = page_translate(addr, false);
        return paddr_read(paddr, len);
    }
}

void vaddr_write(vaddr_t addr, int len, uint32_t data)
{
    if (((addr & PAGE_MASK) + len) > PAGE_SIZE) {
        // uint32_t至多写四字节，此次直接认定至多跨一页
        int len1 = PAGE_SIZE - (addr & PAGE_MASK);
        int len2 = len - len1;

        uint32_t data1_to_write = data & ((1ULL << (len1 * 8)) - 1);
        uint32_t data2_to_write = data >> (len1 * 8);

        paddr_t paddr1 = page_translate(addr, true);
        paddr_write(paddr1, len1, data1_to_write);

        paddr_t paddr2 = page_translate(addr + len1, true);
        paddr_write(paddr2, len2, data2_to_write);
    }
    else
    {
        paddr_t paddr = page_translate(addr, true);
        paddr_write(paddr, len, data);
    }
}
