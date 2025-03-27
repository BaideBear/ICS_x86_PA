#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/
static void instr_execute_1op(){
    operand_read(&opr_src);
    cpu.esp -= data_size/8;
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_DS;
    opr_dest.addr = cpu.esp;
    opr_dest.val = opr_src.val;
    operand_write(&opr_dest);
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, v)

make_instr_func(pusha){
    uint32_t tem = cpu.esp;
    OPERAND rel;
    rel.data_size = 32;
    rel.type = OPR_MEM;
    rel.sreg = SREG_SS;
    
    cpu.esp -= 4;
    rel.val = cpu.eax;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    cpu.esp -= 4;
    rel.val = cpu.ecx;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    cpu.esp -= 4;
    rel.val = cpu.edx;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    cpu.esp -= 4;
    rel.val = cpu.ebx;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    cpu.esp -= 4;
    rel.val = tem;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    cpu.esp -= 4;
    rel.val = cpu.ebp;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    cpu.esp -= 4;
    rel.val = cpu.esi;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    cpu.esp -= 4;
    rel.val = cpu.edi;
    rel.addr = cpu.esp;
    operand_write(&rel);
    
    return 1;
}


make_instr_func(push_i_b){
    OPERAND imm, mem;
    int len = 1;
    
    imm.data_size = 8;
    imm.type = OPR_IMM;
    imm.sreg = SREG_CS;
    imm.addr = eip + 1;
    
    cpu.esp -= data_size/8;
    operand_read(&imm);
    imm.val = sign_ext(imm.val, imm.data_size);
    
    mem.data_size = data_size;
    mem.type = OPR_MEM;
    mem.sreg = SREG_DS;
    mem.addr = cpu.esp;
    mem.val = imm.val;
    operand_write(&mem);
    
    return len + 1;
    
}


/*make_instr_func(push_r_v){
    OPERAND r, Esp;
    int len = 1;
    
    r.data_size = 32;
    r.type = OPR_REG;
    r.addr = opcode & 0x07;
    
    Esp.data_size = 32;
    Esp.type = OPR_REG;
    Esp.addr = 4;
    
    operand_read(&Esp);
    Esp.val -= data_size/8;
    operand_write(&Esp);
    
    operand_read(&r);
    OPERAND set;
    set.type = OPR_MEM;
    set.data_size = data_size;
    set.addr = Esp.val;
    set.val = r.val & (0xffffffff >> (32-data_size));
    operand_write(&set);
    
    return len;
}*/
