#ifndef _2MACHEXE_H_
#define _2MACHEXE_H_

#include "2mach.def.h"

#define EXE_SIG (('2'<<24) | ('M'<<16) | ('C'<<8) | ('H'<<0))

typedef unsigned char bitsize_t;

struct prog_atom
{
	enum prog_instr instr : PROG_IINSTR_BITS;
	prog_param_t param : PROG_IPARAM_BITS;
};

struct prog_info
{
	offset_t prog_start; // initial starting vector
	size_t instr_count; // number of instructions;
	bitsize_t addr_width; // memory address width
	bitsize_t instr_size; // instruction size in bits
}

struct prog_exe
{
	struct prog_info *info;
	struct prog_atom *instr; // instruction sequence
};


#define READ_PROG_EXE_EALLOC 1
#define READ_PROG_EXE_EINSIG 2
#define READ_PROG_EXE_ERDINF 3
int
read_prog_exe(FILE *f, struct prog_exe *prog);


#define WRITE_PROG_EXE_EWRSIG 1
#define WRITE_PROG_EXE_EWRINF 2
#define WRITE_PROG_EXE_EWRINS 3
int
write_prog_exe(FILE *f, struct prog_exe *prog);


#define CHECK_SIG_EINSIG 1
int
check_sig(FILE *f);

#endif //_2MACHEXE_H_
