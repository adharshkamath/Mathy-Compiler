
%{
	#include <stdio.h> 
	#include <stdlib.h>
    #define YYSTYPE char* 
	#include "parser.tab.h"
	extern int yycolumn;
	extern int yyleng;
%}


WHITESPACE			[ \t\v\f]
SINGLE_CHAR			[=(){};,!]
CONTROL_STMT		forall|product|sigma|where
IDENTIFIER 			[a-zA-Z_]([a-zA-Z0-9_])*
INT_CONSTANT		[0-9]+
FLOAT_CONSTANT 		[1-9][0-9]*(\.[0-9]+)?|0(\.[0-9]+)?
UNARY_OPERATOR 		(\+\+|\-\-)
OPERATOR 			(=|\+|\-|\/|\*|%)
COMPARISON 			>=|>|<|<=
PRECISION			(float|double|long_double)

%%

\n 						{ yycolumn = 0; yylineno++; return yytext[0]; }
{SINGLE_CHAR}   		{ yycolumn++; return yytext[0]; }
{INT_CONSTANT}			{ yycolumn += yyleng; yylval = strdup(yytext); return INT_CONSTANT; }
{FLOAT_CONSTANT}		{ yycolumn += yyleng; return FLOAT_CONSTANT; }
{UNARY_OPERATOR} 		{ yycolumn += yyleng; return UNARY_OPERATOR; }
{OPERATOR} 				{ yycolumn += yyleng; return OPERATOR; }
{COMPARISON}			{ yycolumn += yyleng; yylval = strdup(yytext); return COMPARISON; }
"forall"				{ yycolumn += yyleng; return FORALL; }
"sigma"					{ yycolumn += yyleng; return SIGMA; }
"product"				{ yycolumn += yyleng; return PRODUCT; }
"where"					{ yycolumn += yyleng; return WHERE; }
{PRECISION}				{ yycolumn += yyleng; yylval = strdup(yytext); return PRECISION; }
{IDENTIFIER}			{ yycolumn += yyleng; yylval = strdup(yytext); return IDENTIFIER; }
{WHITESPACE}    		{ yycolumn += yyleng; }
.						{ yycolumn += 1; return yytext[0];}


%%
  
int yywrap() {  
	return -1;  
}
