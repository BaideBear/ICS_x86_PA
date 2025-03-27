#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	
	return cpu.segReg[sreg].base + offset;
}
// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	 SegDesc *addr = (SegDesc*)((uint32_t)hw_mem + cpu.gdtr.base + 8*cpu.segReg[sreg].index);
	 uint32_t base = (addr->base_31_24 << 24) + (addr->base_23_16 << 16) + addr->base_15_0;
	 uint32_t limit = (addr->limit_19_16 << 16) + addr->limit_15_0;
	 assert(base == 0);
	 assert(limit == 0xfffff);
	 assert(addr->granularity == 1);
	 cpu.segReg[sreg].base = base;
	 cpu.segReg[sreg].limit = limit;
	 cpu.segReg[sreg].privilege_level = addr->privilege_level;
}
