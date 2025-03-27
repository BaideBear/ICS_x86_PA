#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/

make_instr_func(leave){
    OPERAND esp, ebp, r;
    
    esp.type = OPR_REG;
    esp.data_size = 32;   //这个data_size的长度不是很确定，存疑
    esp.addr = 0x4;
    
    ebp.type = OPR_REG;
    ebp.data_size = 32;
    ebp.addr = 0x5;
    
    r.type = OPR_MEM;
    r.data_size = 32;
    r.sreg = SREG_SS;
    
    operand_read(&ebp);
    esp.val = ebp.val;   //ebp -> esp
    
    r.addr = esp.val;
    operand_read(&r);
    ebp.val = r.val;
    esp.val += 4;
    operand_write(&ebp);  // pop() -> ebp
    operand_write(&esp);
    
    
    return 1;
};
