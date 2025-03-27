#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
*/
static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val = sign_ext(opr_src.val,opr_src.data_size);
	opr_dest.val = sign_ext(opr_dest.val,opr_dest.data_size);
	alu_sub(opr_src.val,opr_dest.val,data_size);
}

make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, rm, bv)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, i, a, v)





/*make_instr_func(cmp_i2rm_bv){
    OPERAND imm,rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip+1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    operand_read(&imm);
    operand_read(&rm);
    if((imm.val&0x80) != 0){
        imm.val = imm.val | 0xffffff00;
    }
    alu_sub(imm.val, imm.val, data_size);
    
    return len + 1;
}

make_instr_func(cmp_i2a_b){
    OPERAND imm, eax; 
    int len = 1;
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    eax.type = OPR_REG;
    eax.addr = 0x0;
    eax.data_size = 8;
    
    operand_read(&eax);
    operand_read(&imm);
    alu_sub(imm.val, eax.val, 8);
    
    return len + 1;
}*/