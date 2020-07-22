frontend: lexer parser
	gcc lex.yy.c parser.tab.c -o test

lexer: lexer.l
	flex lexer.l

parser: parser.y
	bison -d parser.y