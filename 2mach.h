#ifndef _2MACH_H_
#define _2MACH_H_

#include "2mach.def.h"

cell_t *
read_mem(offset_t offset);

void
write_mem(offset_t offset, cell_t cell);

struct progdata *
read_prog(offset_t offset);

void
exec_cycle(void);

void
add_instr(void *cell);

void
cond_instr(void *jump);

static void (*instr_tab[2])(void *) = { add_instr, cond_instr };

#endif //_2MACH_H_
