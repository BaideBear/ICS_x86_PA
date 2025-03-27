#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
make_instr_func(call_near){
    OPERAND ip, rel;
    
    rel.data_size = data_size;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.addr = cpu.eip+1;
    operand_read(&rel);
    int offset = sign_ext(rel.val, data_size);
    
  
    ip.data_size = data_size;
    ip.type = OPR_MEM;
    ip.sreg = SREG_DS;
    ip.val = cpu.eip+1+data_size/8;
    
    cpu.esp -= data_size/8;
    ip.addr = cpu.esp;
    operand_write(&ip);
    cpu.eip += offset;
    
    return 1 + data_size/8;
    
}
make_instr_func(call_near_indirect){
    OPERAND ip, rel;
    int len = 1;
    
    rel.data_size = data_size;
    len += modrm_rm(eip+1, &rel);
    operand_read(&rel);
    
    cpu.esp -= data_size/8;
    
    ip.data_size = data_size;
    ip.type = OPR_MEM;
    ip.addr = cpu.esp;
    ip.val = cpu.eip + len;
    operand_write(&ip);
    
    if(data_size == 16){
        cpu.eip = rel.val & 0xffff;
    }
    else{
        cpu.eip = rel.val;
    }
    
    return 0;
}



















