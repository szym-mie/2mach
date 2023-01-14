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

progdata_t *
read_prog(offset_t offset)
{
	return prog + (offset & PROG_BITS);
}

int
load_prog_stdin(void)
{
	offset_t ic = 0;

	char line_buf[256] = { 0 };
	
	int c = 0;
	while ((c = getchar()) != EOF)
	{
		if (
	}

	return 0;
}

int
read_line_stdin(char *buf, int max_size)
{
	int size = 0;
	int c = 0;

	while ((c = getchar()) != EOF)
	{
		if (c == '\n')
			*(buf++) = 	

		if (++size >= max_size)
			return max_size;
	}
	
	return size;
}
