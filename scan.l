%{
	#include "./scan.h"
	
%}

DIGIT	[0-9]
IDR	[a-zA-Z][a-zA-Z0-9]*

%%

; { return SEMICOLON; }
"(" { return LPARAN; }
")" { return RPARAN; }
"{" { return LBRACE; }
"}" { return RBRACE; }
"<" { return LT; }
">" { return GT; }
"==" { return EQ; }
= { return ASSIGN; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIVIDE; }
"while" { return WHILE; }
"if" { return IF; } 
"else" { return ELSE; }
"int" { return INT; }
"float" { return FLOAT; }
{IDR} {  return ID; }
{DIGIT}+ { return ICONST; }
[ \t\n]+ { }
{DIGIT}+"."{DIGIT}* { return FCONST; }	
"."{DIGIT}+ { return FCONST; }
. { return ERROR; } 

%%

/*Token scan() {
  yyin = stdin;
  Token res;
  res.type = yylex();
  if (res.type == 0)
      res.type = END;
  res.value = yytext;
  res.len = yyleng;
  return res;
}*/
