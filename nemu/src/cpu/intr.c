#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	OPERAND rel;
	rel.sreg = SREG_SS;
	rel.type = OPR_MEM;
	rel.data_size = 32;
	
	cpu.esp -= 4;
	rel.addr = cpu.esp;
	rel.val = cpu.eflags.val;
	operand_write(&rel);
	
	cpu.esp -= 4;
	rel.addr = cpu.esp;
	rel.val = cpu.segReg[1].val;
	operand_write(&rel);
	
	cpu.esp -= 4;
	rel.addr = cpu.esp;
	rel.val = cpu.eip;
	operand_write(&rel);
	
	GateDesc *gatedesc = (GateDesc*)(hw_mem + page_translate(segment_translate(cpu.idtr.base + 8*intr_no, SREG_CS)));
	if(gatedesc->type == 0xe){
	    cpu.eflags.IF = 0;
	}
	uint32_t addr = (gatedesc->offset_31_16 << 16) + gatedesc->offset_15_0;
	cpu.segReg[1].val = gatedesc->selector;
	cpu.eip = addr;
	
	
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
