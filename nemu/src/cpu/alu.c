#include "cpu/cpu.h"


void set_PF(uint32_t res) {
	int num = 0;
	for (int i = 0; i < 8; ++i) {
		if (res % 2 == 1) {
			num++;
		}
		res >>= 1;
	}
	if (num % 2 == 0) {
		cpu.eflags.PF = 1;
	}
	else {
		cpu.eflags.PF = 0;
	}
}

void set_ZF(uint32_t res, size_t data_size) {
	res = res & (0xFFFFFFFF >> (32 - data_size));
	if (res == 0) {
		cpu.eflags.ZF = 1;
	}
	else {
		cpu.eflags.ZF = 0;
	}
}

void set_SF(uint32_t res, size_t data_size) {
	res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(res);

}




/*
void set_ZF(uint32_t result, size_t data_size) {
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF = (result == 0);
}

void set_SF(uint32_t result, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result) {
	int one_num = 0;
	for (int i = 0; i < 8; i++) {
		one_num += result % 2;
		result = result >> 1;
	}
	cpu.eflags.PF = (one_num % 2 == 0);
}
*/

void set_CF_add(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src;
}
void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {   //considered as set_OF_adc
	switch (data_size) {
	case 8:
		result = sign_ext(result & 0xFF, 8);
		src = sign_ext(src & 0xFF, 8);
		dest = sign_ext(dest & 0xFF, 8);
		break;
	case 16:
		result = sign_ext(result & 0xFFFF, 16);
		src = sign_ext(src & 0xFFFF, 16);
		dest = sign_ext(dest & 0xFFFF, 16);
		break;
	default: break;// do nothing
	}
	if (sign(src) == sign(dest)) {
		if (sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
	else {
		cpu.eflags.OF = 0;
	}
}


uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	uint32_t res = 0;
	res = dest + src;

	set_CF_add(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);


	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}


void set_CF_adc(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = (result <= src && cpu.eflags.CF) || (result < src && !cpu.eflags.CF);
}
uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	uint32_t res = src + dest + cpu.eflags.CF;

	set_CF_adc(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}


void set_CF_sub(uint32_t result, uint32_t dest, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result > dest;
}
void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {//considered as set_OF_sbb
	switch (data_size) {
	case 8:
		result = sign_ext(result & 0xFF, 8);
		src = sign_ext(src & 0xFF, 8);
		dest = sign_ext(dest & 0xFF, 8);
		break;
	case 16:
		result = sign_ext(result & 0xFFFF, 16);
		src = sign_ext(src & 0xFFFF, 16);
		dest = sign_ext(dest & 0xFFFF, 16);
		break;
	default: break;// do nothing
	}
	if (sign(src) == sign(dest)) {
		cpu.eflags.OF = 0;
	}
	else {
		if (sign(dest) == sign(result))
			cpu.eflags.OF = 0;
		else
			cpu.eflags.OF = 1;
	}
}
uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	uint32_t res = dest - src;

	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_CF_sub(res, dest, data_size);
	set_OF_sub(res, src, dest, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}



void set_CF_sbb(uint32_t result, uint32_t dest, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = (result > dest && !cpu.eflags.CF) || (result >= dest && cpu.eflags.CF);
}


uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	uint32_t res = dest - (src + cpu.eflags.CF);

	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_CF_sbb(res, dest, data_size);
	set_OF_sub(res, src, dest, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}


void set_CF_mul(uint64_t res, size_t data_size) {
	res = res >> data_size;
	cpu.eflags.CF = (res != 0);
}
void set_OF_mul(uint64_t res, size_t data_size) {
	res = res >> data_size;
	cpu.eflags.OF = (res != 0);
}
uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	//dest =  dest & (0xFFFFFFFF >> (32-data_size));
	uint64_t res = (uint64_t)src * (uint64_t)dest;

	set_OF_mul(res, data_size);
	set_CF_mul(res, data_size);

	return res;
#endif
}



int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)    //很可能标志位有bug，没修改标志位就通过测评了
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	int64_t a = sign_ext_64(src, data_size), b = sign_ext_64(dest, data_size);
	int64_t res = a * b;

	set_OF_mul(res, data_size);
	set_CF_mul(res, data_size);  //可能不太对

	return res;
#endif
}



// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	assert(src != 0);
	uint32_t res = dest / src;


	return res;
#endif
}



// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	assert(src != 0);
	int32_t res = dest / src;

	return res;
#endif
}



uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	assert(src != 0);
	uint32_t res = dest % src;



	return res;
#endif
}



int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	assert(src != 0);
	int32_t res = dest % src;

	return res;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	uint32_t res = src & dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	uint32_t res = src ^ dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}



uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	uint32_t res = src | dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}


void set_CF_shl(uint32_t src, uint32_t dest, size_t data_size) {
	dest = dest >> (data_size - src);
	cpu.eflags.CF = dest % 2;
}
void set_OF_shl(uint32_t src, uint32_t dest, size_t data_size) {
	if (src == 1) {
		dest = dest >> (data_size - 1);
		cpu.eflags.OF = dest % 2;
	}
}
uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	uint32_t res = dest << src;

	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	set_CF_shl(src, dest, data_size);
	set_OF_shl(src, dest, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}


void set_CF_shr(uint32_t src, uint32_t dest, size_t data_size) {
	dest = dest >> (src - 1);
	cpu.eflags.CF = dest % 2;
}
void set_OF_shr(uint32_t src, uint32_t dest, size_t data_size) {
	if (src == 1) {
		dest = dest >> (data_size - 1);
		cpu.eflags.OF = dest % 2;
	}
}
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	uint32_t res = (dest & (0xFFFFFFFF >> (32 - data_size))) >> src;

	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	set_CF_shr(src, dest, data_size);
	set_OF_shr(src, dest, data_size);


	return res;
#endif
}



uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

	uint64_t res = sign_ext_64(dest, data_size);
	res = res >> src;

	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	set_CF_shr(src, dest, data_size);
	set_OF_shr(src, dest, data_size);

	return ((uint32_t)res) & (0xFFFFFFFF >> (32 - data_size));
#endif
}


uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	return alu_shl(src, dest, data_size);
#endif
}