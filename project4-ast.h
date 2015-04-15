/*=================================
File project4-ast.h
================================*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* ============== types and functions dealing with variables and symbol table */

char* TYPE_NAME[] = {"TYPE_void", "TYPE_int", "TYPE_float", "TYPE_unknown"} ;

typedef enum {TYPE_void, TYPE_int, TYPE_float, TYPE_unknown} Type_enum;

typedef struct SymbolTableEntry {
	char* var_name;
	Type_enum var_type;
	struct SymbolTableEntry* next;
} SymbolTableEntry;

typedef struct SymbolTable {
	SymbolTableEntry* entry;
	struct SymbolTable* prev;
} SymbolTable;

/* the current block being processed*/
SymbolTable* currentblock = 0;

/* entering a new block */
void push_block() {
	SymbolTable* newblock = (SymbolTable*) malloc(sizeof(SymbolTable));
	newblock->entry = 0;
	newblock->prev = currentblock;
	currentblock = newblock;
}

/* exiting the current block*/
void pop_block() {
	if (currentblock == NULL) {
		printf("ERROR: there is no block to pop\n");
		exit(-1);
	}
	currentblock = currentblock->prev;
}


/* look for the symbol table entry for a given variable*/
SymbolTableEntry* find_var(const char* name, int len) {
	SymbolTable* p = currentblock;
	while (p != NULL) {
		SymbolTableEntry* res = p->entry;
		while (res != NULL) {
			if (!memcmp(res->var_name, name, len))
				return res;
			res = res->next;
		}
		p = p->prev;
	}
	printf("ERROR: undefined variable: ");
	int i;
	for (i = 0; i < len; ++i)
	printf("%c", name[i]);
	printf("\n");
	exit(-1);
}


/* create a new variable with the given type in the current block*/
SymbolTableEntry* new_varDecl(char* name, int len, Type_enum var_type) {
	if (currentblock == 0)
		push_block();
	SymbolTableEntry* res = currentblock->entry, *prev = 0;
	while (res != NULL) {
		printf("debug\n\n\n\n\n\n\n\n\n");
		if (!memcmp(res->var_name, name, len)) {
		printf("ERROR: variable %s has already been defined in the same block\n", res->var_name);
		exit(-1);
		}
	prev = res;
	res = res->next;
	}
	res = (SymbolTableEntry*) malloc(sizeof(SymbolTableEntry));
	res->next = NULL;
	if (prev == NULL)
		currentblock->entry = res;
	else
	prev->next = res;
	res->var_name = (char*)malloc((len+1) * sizeof(char));
	memcpy(res->var_name, name, len);
	res->var_name[len] = 0;
	res->var_type = var_type;

	return res;
}


/*=============== types and functions dealing with AST ===================*/
char* AST_NAME[]={	"FUNC_DEFN", "STMT_BLOCK", "STMT_LIST",
			"STMT_DECL", "STMT_ASSIGN", "STMT_WHILE", "STMT_IF",
			"EXP_UMINUS", "EXP_PLUS", "EXP_MINUS", "EXP_MULT",
			"EXP_DIVIDE", "EXP_LT", "EXP_GT", "EXP_EQ",
			"ID", "ICONST", "FCONST",
			"TYPE_void", "TYPE_int", "TYPE_float"};


typedef enum {		FUNC_DEFN, STMT_BLOCK, STMT_LIST,
			STMT_DECL, STMT_ASSIGN, STMT_WHILE, STMT_IF,
			EXP_UMINUS, EXP_PLUS, EXP_MINUS, EXP_MULT,
			EXP_DIVIDE, EXP_LT, EXP_GT, EXP_EQ,
			AST_VAR, AST_ICONST, AST_FCONST} AST_enum;

typedef struct AST {
	AST_enum tag;
	union {
		struct Operator {
			struct AST *op1, *op2, *op3;
		Type_enum op_type;
		} operator;
	SymbolTableEntry* entry; /* if AST is a variable*/
	int int_value; /* if AST is ICONST */
	char* float_value; /* if AST is FCONST */
	} content;
} AST;

/* print the content of AST node; col indicates the colume to start printing*/
void print_AST(AST* node, int col) {
	if (node == NULL) return;
	int i;
	for (i = 0; i < col; ++i)
	printf (" ");
	printf(AST_NAME[node->tag]);
	switch (node->tag) {
		case AST_VAR:
			printf("(%s : %s)\n", node->content.entry->var_name,
			TYPE_NAME[node->content.entry->var_type]);
			break;
		case AST_ICONST:
			printf("(%d)\n", node->content.int_value);
			break;
		case AST_FCONST:
			printf("(%s)\n", node->content.float_value);
			break;
		default:
			printf(" : %s \n", TYPE_NAME[node->content.operator.op_type]);
			print_AST(node->content.operator.op1, col+2);
			print_AST(node->content.operator.op2, col+2);
			print_AST(node->content.operator.op3, col+2);
			break;
		}
}

AST* new_var (const char* name, int len) {
	AST* res = (AST*)malloc(sizeof(AST));
	res->tag = AST_VAR;
	res->content.entry = find_var(name,len);
	return res;
}


AST* new_ICONST (int val) {
	AST* res = (AST*)malloc(sizeof(AST));
	res->tag = AST_ICONST;
	res->content.int_value = val;
	return res;
}


AST* new_FCONST (char* text, int len) {
	char* value = (char*)malloc((len+1) * sizeof(char));
	memcpy(value, text, len);
	value[len] = 0;
	AST* res = (AST*)malloc(sizeof(AST));
	res->tag = AST_FCONST;
	res->content.float_value = value;
	return res;
}


AST* new_operator(AST_enum tag, AST* op1, AST* op2, AST* op3, Type_enum op_type) {
	AST* res = (AST*)malloc(sizeof(AST));
	res->tag = tag;
	res->content.operator.op_type = op_type;
	res->content.operator.op1 = op1;
	res->content.operator.op2 = op2;
	res->content.operator.op3 = op3;
	return res;
}
