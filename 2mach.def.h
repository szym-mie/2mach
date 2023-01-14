#ifndef _2MACH_DEF_H_
#define _2MACH_DEF_H_


// common config for machine and assembler
#define MEM_ADDR_BITS 31
#define INSTR_SIZE 32

typedef unsigned int cell_t;
typedef unsigned int prog_param_t;
typedef unsigned int offset_t;

#define PROG_IINSTR_BITS (INSTR_SIZE - MEM_ADDR_BITS)
#define PROG_IPARAM_BITS MEM_ADDR_BITS 

enum prog_instr
{
	ADD, 
	COND
};


#endif //_2MACH_DEF_H_
