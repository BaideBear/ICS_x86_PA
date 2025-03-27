#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/
make_instr_func(ret_near){
    OPERAND ip, esp;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    operand_read(&esp);
    
    ip.data_size = 32;
    ip.type = OPR_MEM;
    ip.sreg = SREG_DS;
    ip.addr = esp.val;
    
    operand_read(&ip);
    cpu.eip = ip.val;
    esp.val += 4;
    operand_write(&esp);
    
    return 0;
}
make_instr_func(ret_near_imm16){
    OPERAND ip, esp, imm;
    
    imm.data_size = 16;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    operand_read(&imm);
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    operand_read(&esp);
    
    ip.data_size = data_size;
    ip.type = OPR_MEM;
    ip.sreg = SREG_SS;
    ip.addr = esp.val;
    operand_read(&ip);
    cpu.eip = ip.val;
    
    int offset = sign_ext(imm.val, imm.data_size);
    esp.val += data_size/8 + offset;
    operand_write(&esp);
    
    
    
    return 0;
}


