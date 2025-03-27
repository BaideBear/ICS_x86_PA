#include "cpu/instr.h"
/*
Put the implementations of `dec' instructions here.
*/
static void instr_execute_1op(){
    operand_read(&opr_src);
    
    int temp = 0;
    if(cpu.eflags.CF == 1){
        temp = 1;
    }
    
    opr_src.val = alu_sub(1, opr_src.val, data_size);
    
    if(temp == 1){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
    
    
    operand_write(&opr_src);
}

make_instr_impl_1op(dec, r, v)
make_instr_impl_1op(dec, rm, v)





