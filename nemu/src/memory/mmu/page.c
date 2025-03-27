#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	/*printf("\nPlease implement page_translate()\n");
	fflush(stdout);
	assert(0);*/
	PDE *pde = (PDE*) (hw_mem + (cpu.cr3.pdbr << 12) + 4*((laddr >> 22) & 0x3FF));
	assert(pde->present == 1);
	PTE *pte = (PTE*) (hw_mem + (pde->page_frame << 12) + 4*((laddr >> 12) & 0x3FF));
	assert(pte->present == 1);
	uint32_t paddr = (pte->page_frame << 12) + (laddr & 0xFFF);
	return paddr;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
