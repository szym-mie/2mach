#include <stdio.h>

#include "2mach.h"

static cell_t mem[MEM_SIZE] = { 0 };
static progdata_t prog[PROG_SIZE] = { 0 };

static offset_t pc = 0;
static cell_t acc = 0;

int
main(void)
{
	

	return 0;
}

cell_t *
read_mem(offset_t offset)
{
	return mem + (offset & MEM_BITS);
}

void
write_mem(offset_t offset, cell_t cell)
{
	*read_mem(offset) = cell;
}
