#include "cpu/instr.h"
/*
Put the implementations of `test' instructions here.
*/
static void instr_execute_2op(){
    operand_read(&opr_src);
    operand_read(&opr_dest);
    opr_dest.val = alu_and(opr_src.val, opr_dest.val, data_size);
    
    if(opr_dest.val == 0){
        cpu.eflags.ZF = 1;
    }
     else{
        cpu.eflags.ZF = 0;
    }
    
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    
    
    //operand_write(&opr_dest);
}

make_instr_impl_2op(test, r, rm, v)
make_instr_impl_2op(test, r, rm, b)
make_instr_impl_2op(test, i, rm, v)
make_instr_impl_2op(test, i, a, v)
make_instr_impl_2op(test, i, a, b)
make_instr_impl_2op(test, i, rm, b)





/*
make_instr_func(test_r2rm_v){
    OPERAND r, rm;
    int len = 1;
    
    r.data_size = data_size;
    rm.data_size = data_size;
    len += modrm_r_rm(eip+1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    rm.val = alu_and(r.val, rm.val, data_size);
    operand_write(&rm);
    
    return len;
}*/
