
%{
	#include <stdio.h> 
	#include <stdlib.h>
    #define YYSTYPE char* 
	#include "y.tab.h"
	extern int yycolumn;
	extern int yyleng;
%}


WHITESPACE			[ \t\v\f]
SINGLE_CHAR			[;,!]
CONTROL_STMT		forall|product|sigma|where
IDENTIFIER 			[a-zA-Z_]([a-zA-Z0-9_])*
INT_CONSTANT		[0-9]+
FLOAT_CONSTANT 		[1-9][0-9]*(\.[0-9]+)?|0(\.[0-9]+)?
OPERATOR 			(=|\+|\-|\/|\*|%)
COMPARISON 			<|<=
PRECISION			(float|double|long_double)
LEFT_SQUARE			"["
RIGHT_SQUARE		"]"
LEFT_PAREN			"("
RIGHT_PAREN			")"
LEFT_CURLY			"{"
RIGHT_CURLY			"}"
EQUALS				"="

%%

\n 						{ yycolumn = 0; yylineno++; return yytext[0]; }
{SINGLE_CHAR}   		{ yycolumn++; yylval = strdup(yytext); return yytext[0]; }
{INT_CONSTANT}			{ yycolumn += yyleng; yylval = strdup(yytext); return INT_CONSTANT; }
{FLOAT_CONSTANT}		{ yycolumn += yyleng; yylval = strdup(yytext); return FLOAT_CONSTANT; }
{OPERATOR} 				{ yycolumn += yyleng; yylval = strdup(yytext); return OPERATOR; }
{COMPARISON}			{ yycolumn += yyleng; yylval = strdup(yytext); return COMPARISON; }
"forall"				{ yycolumn += yyleng; return FORALL; }
"sigma"					{ yycolumn += yyleng; return SIGMA; }
"product"				{ yycolumn += yyleng; return PRODUCT; }
"where"					{ yycolumn += yyleng; return WHERE; }
{IDENTIFIER}			{ yycolumn += yyleng; yylval = strdup(yytext); return IDENTIFIER; }
{WHITESPACE}    		{ yycolumn += yyleng; }
{LEFT_SQUARE}			{ yycolumn += yyleng; yylval = strdup(yytext); return LEFT_SQUARE; }
{RIGHT_SQUARE}			{ yycolumn += yyleng; yylval = strdup(yytext); return RIGHT_SQUARE; }
{LEFT_PAREN}			{ yycolumn += yyleng; yylval = strdup(yytext); return LEFT_PAREN; }
{RIGHT_PAREN}			{ yycolumn += yyleng; yylval = strdup(yytext); return RIGHT_PAREN; }
{LEFT_CURLY}			{ yycolumn += yyleng; yylval = strdup(yytext); return LEFT_CURLY; }
{RIGHT_CURLY}			{ yycolumn += yyleng; yylval = strdup(yytext); return RIGHT_CURLY; }
{EQUALS}				{ yycolumn += yyleng; yylval = strdup(yytext); return EQUALS; }
.						{ yycolumn += 1; yylval = strdup(yytext); return yytext[0];}


%%

