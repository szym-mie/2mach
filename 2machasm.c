#include <stdio.h>
#include <stdlib.h>

#include "2machasm.h"

#define ABORT(E) \
	printf(FATAL_MSG); \
	return E;

static char *src_file = "";
static char *out_file = "out";

static struct list *instr_list;
static struct list *label_list; // TODO reimplement using hash table

int
main(int argc, char *argv[])
{
	if (parse_args(argc, argv))
	{
		ABORT(2)
	}

	FILE *fsrc;
	FILE *fout;

	if (*src_file != '\0')
	{
		// read program src from file
		fsrc = fopen(src_file, "r");
		if (fsrc == NULL)
		{
			printf(NOT_FOUND_MSG, src_file);
			ABORT(1)
		}
	}
	else
	{
		// read program src from stdin
		fsrc = stdin;
	}

	fout = fopen(out_file, "w");
	if (fout == NULL)
	{
		printf(NOT_FOUND_MSG, out_file);
		ABORT(1)
	}

	// TODO assemble program
	
	puts("assembling...");

	return 0;
}

int
parse_args(int argc, char *argv[])
{
	int argi = 1;
	for (;;) 
	{
		int stat = parse_arg(argc, argv, &argi);
		switch (stat)
		{	 
			case PARSE_ARG_EEMPTY:
				return 0;
			case PARSE_ARG_ENOVAL:
				printf(NOVAL_MSG, argv[argi]);
				return PARSE_ARGS_EFATAL;
			case PARSE_ARG_EINVAL:
				printf(INVAL_MSG, argv[argi]);
				return PARSE_ARGS_EFATAL;
			case PARSE_ARG_EDHELP:
				printf(DHELP_MSG);
				return 0;
		}
	}

	return 0;
}

int
parse_arg(int argc, char *argv[], int *argi)
{
	if (*argi < argc)
		return PARSE_ARG_EEMPTY;

	char *arg = argv[*argi];
	if (*arg == '-')
	{
		char arg_name = *(arg+1);
		char *arg_val;
		if (++*argi < argc)
			arg_val = argv[*argi];
		else
			return PARSE_ARG_ENOVAL;
		
		switch (arg_name)
		{
			case 'o':
				// set output filename
				out_file = arg_val;
				break;
			case 'h':
				// display help
				return PARSE_ARG_EDHELP;
			default:
				return PARSE_ARG_EINVAL;
		}
	}
	else
	{
		// set src filename
		src_file = arg;
	}
}

int
read_line(FILE *f, char *buf, int max_size)
{
	int size = 0;
	int c = 0;

	while ((c = fgetc(f)) != EOF)
	{
		if (++size >= max_size)
			break;

		if (c == '\n')
			break;

		*buf++ = c;
	}

	*buf = '\0';
	
	return size;
}

/* syntax
 *
 * instruction with parameter
 * 	<instr-name> <param>
 * 	add 0x01
 *
 * instruction with label
 * 	<instr-name> <label>
 * 	cond :start
 *
 * label declaration
 * 	<label> <instr?>
 * 	start: add 0x01 -- declare 'start' for this instruction
 * 	start: \n -- just label use next instruction
 *
 */
int
load_prog(FILE *f)
{
	offset_t ic = 0;
#define LINE_SIZE 256
	char line_buf[LINE_SIZE] = { 0 };
	
	while (read_line(f, line_buf, LINE_SIZE))
	{
		// TODO parser	
	}
#undef LINE_SIZE

	return 0;
}

int
new_label(const char *name, const struct src_instr *loc)
{
	struct src_label *label = malloc(sizeof(struct src_label));
	if (label == NULL)
		return NEW_LABEL_EALLOC;

	label->name = name;
	label->loc = loc;

	return list_add(&label_list, label);
}

struct src_instr *
ref_label(const char *name)
{
	struct list *found = list_find(label_list, name, str_cmp);
	if (found == NULL)
		return NULL;
	
	return found->elem;
}

int
new_instr(const char *name, unsigned int param, const struct src_label *label)
{
	struct src_instr *instr = malloc(sizeof(struct src_instr));
	if (instr == NULL)
		return NEW_INSTR_EALLOC;

	instr->name = name;
	instr->param = param;
	instr->label = label;

	return list_add(&instr_list, instr);
}

int
list_add(struct list **list, void *elem)
{
	struct list *node = malloc(sizeof(struct list));
	if (node == NULL) 
		return LIST_ADD_EALLOC;

	node->next = NULL;
	node->elem = elem;

	if (*list != NULL)
	{
		struct list *lptr = *list;
		while (lptr->next != NULL) 
			lptr = lptr->next;
		lptr->next = node;
	}
	else
	{
		*list = node;
	}

	return 0;
}

struct list *
list_find(struct list *list, void *elem, int (*cmp)(void *, void *))
{
	if (list == NULL)
		return NULL;

	struct list *lptr = list;
	while (lptr != NULL)
	{
		if (test(elem, lptr->elem)) 
			return lptr;
		lptr = lptr->next;
	}

	return NULL;
}

int
tree_add(struct tree **tree, void *elem, int (*cmp)(void *, void *))
{
	struct tree *node = malloc(sizeof(struct tree));
	if (node == NULL) 
		return TREE_ADD_EALLOC;

	node->left = NULL;
	node->right = NULL;
	node->elem = elem;

	if (*tree != NULL)
	{
		int tr;
		struct tree *tptr = *tree;
		while ((tr = test(elem, tptr->elem)) != 0)
		{
			tptr = tr < 0 ? tptr->left : tptr->right;
			if (tptr == NULL)
			{
				tptr = node;
				break;
			}
		}
	}
	else
	{
		*tree = node;
	}

	return 0;
}

struct tree *
tree_find(struct *tree tree, void *elem, int (*cmp)(void *, void *))
{
	int tr;
	struct tree *tptr = tree;
	while ((tr = test(elem, tptr->elem)) != 0)
	{
		tptr = tr < 0 ? tptr->left : tptr->right;
		if (tptr == NULL) 
			return NULL;
	}

	return tptr;
}

struct list *
hash_find(struct hash *table, void *elem)
{
	unsigned int ndx = table->hash(elem) % table->size;
	struct list *list = table->lists[ndx];

	return list_find(list, elem, table->cmp);
}


int
str_cmp(const char *str1, const char *str2)
{
	while (*str1++ == *str2++)
	{
		if (*str1 == '\0')
			return 0;
	}

	return *str1 - *str2 > 0 ? 1 : -1;
}

unsigned long
str_hash(const char *str)
{
	unsigned long long h = 5831;
	char c;

	while ((c = *str++) != '\0') 
		h = ((h << 5) + h) + c;	
	
	return h;
}
