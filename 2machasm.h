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

#define LISTE_MSG \
	"could not initialize lists\n"

#define FATAL_MSG \
	BOLD("fatal error") ": aborting\n"

#define NOT_FOUND_MSG \
	"could not open \'%s\' file\n"
// conf
#define TOKEN_SIZE 64

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
	struct src_instr *loc;
};

enum token_type
{
	INSTR_T,
	INT_T,
	LABEL_REF_T,
	LABEL_DECL_T
};

struct token_ctx
{
	enum token_type type;
	char *qname;
	int val;
};

struct list_node;

struct list
{
	struct list_node *start;
};

struct list_node
{
	struct list_node *next;
	void *elem;
};

struct tree_node;

struct tree
{
	struct tree_node *root;
};

struct tree_node
{
	struct tree *left;
	struct tree *right;
	void *elem;
};

struct hash_map
{
	struct list **lists;
	unsigned int size;
	unsigned long (*hash_fn)(void *);
	int (*cmp_fn)(void *, void *);
};

// macros
#define IS_NUM_START(C) (('0' <= C && C <= '9') || C == '-' || C == '+')

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

char
read_char(FILE *f);

#define LOAD_PROG_SRC_EALLOC 1 // internal alloc failed
#define LOAD_PROG_SRC_EINVAL 2 // invalid syntax
int
load_prog_src(FILE *f);

#define BUILD_PROG_EALLOC 1 // internal alloc failed
#define BUILD_PROG_EUNEND 2 // unexpected end of command/input
#define BUILD_PROG_ENOMEM 3 // no more program memory
#define BUILD_PROG_ENOLAB 4 // no label declaration
#define BUILD_PROG_EDPLAB 5 // duplicate label declaration
#define BUILD_PROG_EUNCOM 6 // unknown command
#define BUILD_PROG_EPSIZE 7 // param out of range
#define BUILD_PROG_EPTYPE 8 // param type is not int or ref
#define BUILD_PROG_ENOCTX 9 // int, label out of context
int
build_prog(void);

int
read_token(const char *token, struct token_ctx *ctx);

#define NEW_LABEL_EALLOC 1
#define NEW_LABEL_EDPLAB 2
int
new_label(const char *name, const struct src_instr *instr);

#define SET_LABEL_EALSET 1
int
set_label(const char *name, const struct src_instr *instr);

struct src_label *
ref_label(const char *name);

#define NEW_INSTR_EALLOC 1
int
new_instr(const char *name, unsigned int param, struct src_label *label);

#define NEW_INSTR_PARAM(NAME, PARAM) new_instr(NAME, PARAM, NULL)
#define NEW_INSTR_LABEL(NAME, LABEL) new_instr(NAME, 0, LABEL)

struct list *
list_init(void);

#define LIST_ADD_EALLOC 1
int
list_add(struct list *list, void *elem);

struct list_node *
list_find(struct list *list, void *elem, int (*cmp_fn)(void *, void *));

void *
list_shift(struct list *list);

void *
list_start(struct list *list);

void *
list_get(struct list_node *node);

void
list_free(struct list *list);

struct tree *
tree_init(void);

#define TREE_ADD_EALLOC 1
int
tree_add(struct tree *tree, void *elem, int (*cmp_fn)(void *, void *));

struct tree_node *
tree_find(struct tree *tree, void *elem, int (*cmp_fn)(void *, void *))

struct hash_map *
hash_map_init(
	unsigned int size, 
	unsigned long (*hash_fn)(void *), 
	int (*cmp_fn)(void *, void *));

struct list_node *
hash_map_find(struct hash_map *map, void *elem);

int
str_cmp(const char *str1, const char *str2);

char *
str_cpy(const char *str);

unsigned long
str_hash(const char *str);

#endif //_2MACHASM_H_
