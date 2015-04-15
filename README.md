# compilers-cs4713-project4
Project 4 from my Compiler Writing class in college (UTSA CS4713)
----
CS 4713 project 4
-Mike Bell


Project is incomplete.



To compile, execute 'make' at a shell
To execute, execute './parser < file.in', where 'file.in' is your input source file

--------------------
||modified grammar||
--------------------

```
program 	::=	funcdefn

funcdefn 	::=	type ID LPARAN RPARAN stmtBlock

stmt 		::=	typeDecl SEMICOLON |
			ID ASSIGN exp SEMICOLON |
			WHILE LPARAN exp RPARAN stmt |
			IF LPARAN exp RPARAN stmt stmtA |
			stmtBlock |
			epsilon

stmtA		::=	ELSE stmt | epsilon

stmtList 	::= stmt stmtList | epsilon

stmtBlock 	::= LBRACE stmtList RBRACE

typeDecl 	::= type ID

type		::=	INT | FLOAT

exp		    ::=	expA expB

expA		::=	expC expD

expB		::=	< expA expB | 
                > expA expB | 
                == expA expB | 
                + expA expB |
			    - expA expB | 
                epsilon

expC		::=	ID | ICONST | FCONST | -exp | (exp)

expD		::=	* expC expD | 
                / expC expD | 
                epsilon
```	
