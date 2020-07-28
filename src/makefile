frontend: lexer parser
	gcc -Wall -g -O0 lex.yy.c y.tab.c helper.c -o test

lexer: lexer.l
	lex lexer.l

parser: parser.y
	yacc -d parser.y --debug --verbose -Wno-yacc -Wall