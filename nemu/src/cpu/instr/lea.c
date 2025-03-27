#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.
*/
make_instr_func(lea){
    OPERAND r, rm;
    int len = 1;
    
    r.data_size = data_size;
    rm.data_size = data_size;
    len += modrm_r_rm(eip+1, &r, &rm);
    r.val = rm.addr;
    operand_write(&r);
    
    return len;
}
