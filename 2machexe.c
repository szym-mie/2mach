#include <stdio.h>
#include <stdlib.h>

#include "2machexe.h"

int
read_prog_exe(FILE *f, struct prog_exe *prog)
{
	if (check_sig(f))
		return READ_PROG_EXE_EINSIG;

	struct prog_info *info = malloc(sizeof(struct prog_info));
	if (info == NULL)
		return READ_PROG_EXE_EALLOC;

	if (fread(info, sizeof(struct prog_info), 1, f) != 1)
		return READ_PROG_EXE_ERDINF;

	size_t instr_size = sizeof(struct prog_atom) * info->instr_count;
	struct prog_atom *instr = malloc(instr_size);
	if (instr == NULL)
		return READ_PROG_EXE_EALLOC;

	if (fread(instr, sizeof(struct prog_atom), info->instr_count, f) != 1)
		return READ_PROG_EXE_ERDINS;

	// just in case, to not leave garbage behind
	free(prog->info);
	free(prog->instr);

	prog->info = info;
	prog->instr = instr;

	return 0;
}


int
write_prog_exe(FILE *f, struct prog_exe *prog)
{
	rewind(f);
	if (fwrite(EXE_SIG, sizeof(char [4]), 1, f) != 1)
		return WRITE_PROG_EXE_EWRSIG;

	if (fwrite(prog->info, sizeof(struct prog_info), 1, f) != 1)
		return WRITE_PROG_EXE_EWRINF;

	size_t atom_count = fwrite(
		prog->instr, 
		sizeof(struct prog_atom), 
		prog->info->instr_count, 
		f
	);

	if (atom_count != prog->info->instr_count)
		return WRITE_PROG_EXE_EWRINS;

	return 0;
}


int
check_sig(FILE *f)
{
	rewind(f);
	unsigned int sig = 0;
	size_t sig_size = fread(&sig, sizeof(char [4]), 1, f);
	if (sig_size != 1 || sig != EXE_SIG)
		return CHECK_SIG_EINSIG;
	
	return 0;
}
