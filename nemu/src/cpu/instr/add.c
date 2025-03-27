#include "cpu/instr.h"
/*
Put the implementations of `add' instructions here.
*/
static void instr_execute_2op(){
    operand_read(&opr_src);
    operand_read(&opr_dest);
    opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
    opr_dest.val = sign_ext(opr_dest.val, opr_dest.data_size);
    opr_dest.val = alu_add(opr_src.val, opr_dest.val, data_size);
    operand_write(&opr_dest);
}

make_instr_impl_2op(add, r, rm, v)
make_instr_impl_2op(add, i, rm, bv)
make_instr_impl_2op(add, i, a, v)
make_instr_impl_2op(add, i, rm, v)







/*make_instr_func(add_r2rm_v){
    OPERAND r, rm;
    int len = 1;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    len += modrm_r_rm(eip+1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    rm.val = alu_add(r.val, rm.val, data_size);
    operand_write(&rm);
    
    return len;
}*/