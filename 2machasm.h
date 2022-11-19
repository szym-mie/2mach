#ifndef _2MACHASM_H_
#define _2MACHASM_H_

#include "2mach.def.h"

// ANSI escape codes
#define ANSI_CSI(N) "\x1b[" #N "m"

#define FONT_NORM ANSI_CSI(0)
#define FONT_BOLD ANSI_CSI(1)

#define BOLD(S) FONT_BOLD #S FONT_NORM 

// messages
#define DHELP_MSG \
	"2mach assembler\n\n" \
	"2machasm [FLAGS] SRC-FILE\n\n" \
	"Available flags:\n" \
	"	" BOLD("-o") "output filename\n" \
	"	" BOLD("-h") "display help\n"

#define INVAL_MSG \
	"unkwown flag: " BOLD("%s") "\n"

#define NOVAL_MSG \
	"flag: " BOLD("%s") " requires value\n"

#define FATAL_MSG \
	BOLD("fatal error") ": aborting\n"

#define NOT_FOUND_MSG \
	"could not open \'%s\' file\n"

// data structures
struct src_instr;
struct src_label;

struct src_instr
{
	const char *name;
	unsigned int param;
	const struct src_label *label; // if NULL use param
};

struct src_label
{
	const char *name;
	const struct src_instr *loc;
};

struct list
{
	struct list *next;
	void *elem;
};

struct tree
{
	struct tree *left;
	struct tree *right;
	void *elem;
};

struct hash
{
	struct list *lists[];
	unsigned int size;
	unsigned long (*hash)(void *);
	int (*cmp)(void *, void *);
};

// functions
#define PARSE_ARGS_EFATAL 1
int
parse_args(int argc, char *argv[]);

#define PARSE_ARG_EEMPTY 1
#define PARSE_ARG_EINVAL 2
#define PARSE_ARG_ENOVAL 3
#define PARSE_ARG_EDHELP 4
int
parse_arg(int argc, char *argv[], int *argi);

#define LOAD_PROG_EINVAL 1 // invalid syntax
#define LOAD_PROG_ENOMEM 2 // no more program memory
#define LOAD_PROG_ENOLAB 3 // no label location
#define LOAD_PROG_EDULAB 4 // duplicate label location
#define LOAD_PROG_EUNCOM 5 // unknown command
#define LOAD_PROG_EPSIZE 6 // param out of range
int
load_prog(FILE *f);

#define NEW_LABEL_EALLOC 1
int
new_label(const char *name, const struct src_instr *instr);

struct src_label *
ref_label(const char *name);

#define NEW_INSTR_EALLOC 1
int
new_instr(const char *name, unsigned int param, struct src_label *label);

#define NEW_INSTR_PARAM(NAME, PARAM) new_instr(NAME, PARAM, NULL)
#define NEW_INSTR_LABEL(NAME, LABEL) new_instr(NAME, 0, LABEL)

int
new_instr_label(const char *name, const struct src_label *label);

#define LIST_ADD_EALLOC 1
int
list_add(struct list **list, void *elem);

struct list *
list_find(struct list *list, void *elem, int (*cmp)(void *, void *));

void *
list_get(struct list *list);

#define TREE_ADD_EALLOC 1
int
tree_add(struct tree **tree, void *elem, int (*cmp)(void *, void *));

struct tree *
tree_find(struct tree *tree, void *elem, int (*cmp)(void *, void *))

struct list *
hash_find(struct hash *table, void *elem);

int
str_cmp(const char *str1, const char *str2);

unsigned long
str_hash(const char *str);

#endif //_2MACHASM_H_
