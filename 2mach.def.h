#ifndef _2MACH_DEF_H_
#define _2MACH_DEF_H_


// common config for machine and assembler
#define MEM_BITS 8
#define MEM_SIZE 256

#define PROG_BITS 15
#define PROG_SIZE 32768

typedef unsigned char cell_t;
typedef unsigned short prog_param_t;
typedef unsigned short offset_t;

#define PROG_IINSTR_BITS 1
#define PROG_IPARAM_BITS 15 

enum prog_instr
{
	ADD, 
	COND
};


#endif //_2MACH_DEF_H_
