#include <stdio.h>
#include <stdlib.h>

#include "2machasm.h"

#define ABORT(E) \
	printf(FATAL_MSG); \
	return E;

static char *src_file = "";
static char *out_file = "out";

static struct list *instr_list;
static struct list *label_list; // TODO reimplement using hash map
static struct list *token_list;

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

	// init lists
	instr_list = list_init();
	label_list = list_init();
	token_list = list_init();
	
	if (instr_list == NULL || label_list == NULL || token_list == NULL)
	{
		printf(LISTE_MSG);
		ABORT(3)
	}

	// reading src
	

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

char
read_char(FILE *f)
{
	int c = fgetc(f);
	return c != EOF ? c : '\0';
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
load_prog_src(FILE *f)
{
	char token_buf[TOKEN_SIZE] = "";
	struct token_ctx *token_ctx;

	unsigned int i = 0;
	int read_stat;

	char c;
	while (c = read_char(f))
	{

		if (c == ' ' || c == '\n' || c == '\0')
		{
			if (ti == 0) 
				continue;

			token_buf[i] = '\0';
			i = 0;
			token_ctx = malloc(sizeof(struct token_ctx));
			if (token_ctx == NULL)
				return LOAD_PROG_SRC_EALLOC;
			
			read_stat = read_token(token_buf, token_ctx);
			if (read_stat != 0) 
				return read_stat;

			if (list_add(token_list, token_ctx) == LIST_ADD_EALLOC)
				return LOAD_PROG_SRC_EALLOC;
		
			if (c == '\0')
				return 0;
		}
		else
		{
			token_buf[i++] = c;
		}
	}

	return 0;
}

int
build_prog(void)
{
	struct token_ctx *ctx
	struct token_ctx *next_ctx;
	while ((ctx = list_shift(token_list)) != NULL)
	{
		switch (ctx->type)
		{
			case INSTR_T:
				next_ctx = list_shift(token_list);
				if (next_ctx == NULL)
					// TODO
				new_instr();
				break;
			case INT_T:

				break;
			case LABEL_REF_T:
				
				break;
			case LABEL_DECL_T:

				break;
		}
	}
}

int
read_token(const char *token, struct token_ctx *ctx)
{
	char *str = token;
	int is_num = IS_NUM_START(*str);

	if (is_num)
	{
		token_ctx->type = INT_T;
		token_ctx->val = 0;	
		token_ctx->qname = NULL;

		int stat = 0;

		while (*str == '0')
			*str++;

		switch (*str)
		{
			case 'b':
			case 'B':
				// no support for binary currently
				break;
			case 'x':
			case 'X':
				stat = sscanf(token, "%x", &token_ctx->val);
				break;
			default:
				stat = sscanf(token, "%d", &token_ctx->val);
				break;
		}

		if (stat == 0)
			return LOAD_PROG_SRC_EINVAL; 
	}
	else
	{
		// assume it's an instruction
		token_ctx->type = INSTR_T;

		if (*str == ':')
			token_ctx->type = LABEL_REF_T;
		
		char qname = malloc(str_len(token, TOKEN_SIZE) + 1);
		char c;
		while ((c = *str++) != '\0')
		{	
			if (c == ':')
			{
				if (*str == '\0')
					token_ctx->type = LABEL_DECL_T;
				else
					return LOAD_PROG_SRC_EINVAL;
			}
			else
			{
				*qname++ = c;
			}
		}

		*qname = '\0';

		token_ctx->qname = qname;
	}

	return 0;
}

int
new_label(const char *name, const struct src_instr *loc)
{
	// if duplicate abort
	if (ref_label(name) != NULL)
		return NEW_LABEL_EDPLAB; 

	struct src_label *label = malloc(sizeof(struct src_label));
	if (label == NULL)
		return NEW_LABEL_EALLOC;

	label->name = name;
	label->loc = loc;

	return list_add(label_list, label);
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

	return list_add(instr_list, instr);
}

struct list *
list_init(void)
{
	struct list *list = malloc(sizeof(struct list));
	list->start = NULL;
	
	return list;
}

int
list_add(struct list *list, void *elem)
{
	struct list_node *node = malloc(sizeof(struct list_node));
	if (node == NULL) 
		return LIST_ADD_EALLOC;

	node->next = NULL;
	node->elem = elem;

	if (list->start != NULL)
	{
		struct list_node *lptr = list->start;
		while (lptr->next != NULL) 
			lptr = lptr->next;
		lptr->next = node;
	}
	else
	{
		list->start = node;
	}

	return 0;
}

void *
list_shift(struct list *list)
{
	void *elem = NULL;
	if (list->start != NULL)
	{
		struct list_node *next;
		elem = list->start->elem;
		next = list->start->next;
		free(list->start);
		list->start = next;
	}

	return elem;
}

struct list_node *
list_find(struct list *list, void *elem, int (*cmp_fn)(void *, void *))
{
	struct list *lptr = list->start;
	while (lptr != NULL)
	{
		if (test(elem, lptr->elem)) 
			return lptr;
		lptr = lptr->next;
	}

	return NULL;
}

void *
list_get(struct list_node *node)
{
	return node->elem;
}

void
list_free(struct list *list)
{
	struct list_node *node = list->start;
	while (node != NULL)
	{
		struct list_node *next = node->next;
		free(node->elem);
		free(node);
		node = next;
	}
}

struct tree *
tree_init(void)
{
	struct tree *tree = malloc(sizeof(struct tree));
	tree->root = NULL;

	return tree;
}

int
tree_add(struct tree *tree, void *elem, int (*cmp_fn)(void *, void *))
{
	struct tree_node *node = malloc(sizeof(struct tree_node));
	if (node == NULL) 
		return TREE_ADD_EALLOC;

	node->left = NULL;
	node->right = NULL;
	node->elem = elem;

	if (tree->root != NULL)
	{
		int tr;
		struct tree_node *tptr = tree->root;
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
		tree->root = node;
	}

	return 0;
}

struct tree_node *
tree_find(struct tree *tree, void *elem, int (*cmp_fn)(void *, void *))
{
	int tr;
	struct tree_node *tptr = tree->root;
	while ((tr = test(elem, tptr->elem)) != 0)
	{
		tptr = tr < 0 ? tptr->left : tptr->right;
		if (tptr == NULL) 
			return NULL;
	}

	return tptr;
}

struct hash_map *
hash_map_init(
	unsigned int size, 
	unsigned long (*hash_fn)(void *), 
	int (*cmp_fn)(void *, void *))
{
	struct hash_map *map = malloc(sizeof(struct hash_map));
	if (map == NULL)
		return NULL;

	map->lists = malloc(sizeof(struct list *) * size);
	if (map->lists == NULL)
		return NULL;

	for (unsigned int i = 0; i < size; i++)
	{
		struct list *list = list_init();
		if (list == NULL)
			return NULL;

		*(map->lists+i) = list;
	}

	map->size = size;
	map->hash_fn = hash_fn;
	map->cmp_fn = cmp_fn;

	return map;
}

struct list_node *
hash_map_find(struct hash_map *map, void *elem)
{
	unsigned int ndx = map->hash(elem) % map->size;
	struct list *list = map->lists[ndx];

	return list_find(list, elem, map->cmp);
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

unsigned int
str_len(const char *str, unsigned int max_size)
{
	unsigned int count = 0;
	while (*str++)
	{
		if (count < max_size)
			count++;
	}

	return count;
}

char *
str_cpy(const char *str, unsigned int max_size)
{
	unsigned int len = str_len(str, max_size);
	char *new_str = malloc(len + 1);
	if (new_str == NULL)
		return NULL;

	for (unsigned int i = 0; i < len; i++)
		*(new_str+i) = *(str+i);

	*(new_str+len) = '\0';

	return new_str;
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
