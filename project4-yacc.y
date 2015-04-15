/*=========================================
File project4-yacc.y
============================================*/

%token WHILE IF ELSE RETURN INT FLOAT ID ICONST FCONST ERROR END
%token SEMICOLON
%token LPARAN RPARAN
%token LBRACE RBRACE
%nonassoc ASSIGN
%nonassoc LT GT EQ
%left PLUS MINUS MULT DIVIDE
%{
	#include "./project4-ast.h"
	AST* programAST = NULL;
	#define YYSTYPE AST*
	int yydebug = 1;
	#define YYDEBUG 1
	extern char* yytext;
	extern int yyleng;
	Type_enum temptype;
%}

%%

program : 			{ push_block(); }	
		funcdefn	
				{ pop_block();  programAST = $2;  }

funcdefn : 	type  ID 	{ new_varDecl(yytext, yyleng, (Type_enum)$1);
				  $2 = new_var(yytext, yyleng); }
		LPARAN RPARAN stmtBlock
				{ $$ = new_operator(FUNC_DEFN, $3, $6, 0, (Type_enum)$1); }

stmt : typeDecl SEMICOLON 	{ $$ = $1; }		
	| ID { $1 = new_var(yytext, yyleng);} ASSIGN exp SEMICOLON { $$ = new_operator(STMT_ASSIGN, $1, $4, 0, TYPE_void); }		
	| WHILE LPARAN exp RPARAN stmt 	
	| IF LPARAN exp RPARAN stmt stmtA 	
	| stmtBlock 				

stmtA :	ELSE stmt 
	| 

stmtList : stmt stmtList 	{ if($2==NULL) $$ = new_operator(STMT_LIST, $1, 0, 0, TYPE_void); 
				  	    else $$ = new_operator(STMT_LIST, $1, $2, 0, TYPE_void); }
	|

stmtBlock : LBRACE		{ push_block(); }	
	    stmtList 
	    RBRACE		{ pop_block(); $$ = new_operator(STMT_BLOCK, $3, 0, 0, TYPE_void); }

typeDecl : type ID		{ new_varDecl(yytext, yyleng, (Type_enum)$1);
				  $$ = new_var(yytext,yyleng); }	

type : 		INT		{ $$ = (AST *) TYPE_int; }
		| FLOAT		{ $$ = (AST *) TYPE_float; }

exp : expA expB			{ if ($1->content.operator.op_type >= $2->content.operator.op_type)
				    temptype = $1->content.operator.op_type;
				  else temptype = $2->content.operator.op_type;
				  $$ = new_operator((AST_enum)$2, $1, $2, 0, temptype);  }

expA : expC expD		{ if ($2==NULL) $$ = $1;
				  }

expB : LT expA expB 		
	| GT expA expB			
	| EQ expA expB		
	| PLUS expA expB	{ temptype = $2->content.operator.op_type;
				  if ($3!=NULL) {
				    if (temptype < $3->content.operator.op_type) 
				      temptype = $3->content.operator.op_type;
				  $$ = new_operator(EXP_PLUS, $2, $3, 0, temptype); } }
	| MINUS expA expB	
	|

expC : ID 			{ $$ = $1; }
	| ICONST 		{ $$ = new_ICONST(atoi(yytext)); }
	| FCONST 		{ $$ = new_FCONST(yytext,yyleng); }
	| MINUS exp 		{ $$ = new_operator(EXP_UMINUS, $2, 0, 0, $2->content.operator.op_type); }
	| LPARAN exp RPARAN	{ $$ = $2; }

expD : MULT expC expD 		
	| DIVIDE expC expD 	
	|
; 


%%
#include "./lex.yy.c"
int main()
{
yyparse();
if (programAST != NULL) {
print_AST(programAST,0);
}
return 0;
}
