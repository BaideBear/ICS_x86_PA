#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op(){
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_DS;
    opr_dest.addr = cpu.esp;
    operand_read(&opr_dest);
    opr_src.val = opr_dest.val;
    operand_write(&opr_src);
    cpu.esp += 4;
}

make_instr_impl_1op(pop, r, v);

make_instr_func(popa){
    OPERAND rel;
    rel.data_size = 32;
    rel.type = OPR_MEM;
    rel.sreg = SREG_SS;
    
    rel.addr = cpu.esp;
    operand_read(&rel);
    cpu.edi = rel.val;
    cpu.esp += 4;
    
    rel.addr = cpu.esp;
    operand_read(&rel);
    cpu.esi = rel.val;
    cpu.esp += 4;
    
    rel.addr = cpu.esp;
    operand_read(&rel);
    cpu.ebp = rel.val;
    cpu.esp += 4;
    
    cpu.esp += 4;
    
    rel.addr = cpu.esp;
    operand_read(&rel);
    cpu.ebx = rel.val;
    cpu.esp += 4;
    
    rel.addr = cpu.esp;
    operand_read(&rel);
    cpu.edx = rel.val;
    cpu.esp += 4;
    
    rel.addr = cpu.esp;
    operand_read(&rel);
    cpu.ecx = rel.val;
    cpu.esp += 4;
    
    rel.addr = cpu.esp;
    operand_read(&rel);
    cpu.eax = rel.val;
    cpu.esp += 4;
    
    return 1;
}



