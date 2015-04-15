all: 
	lex scan.l
	yacc -v project4-yacc.y
	gcc -g -o parser y.tab.c -ly -ll

clean:
	rm -f -v parser y.tab.c lex.yy.c *~ *.o
	
