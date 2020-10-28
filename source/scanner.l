%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "parser.hpp"
	#include "location.hh"
    #include "compiler.h"

	using namespace std;

	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);
%}

%option nodefault yylineno
%option c++
%option yyclass="Scanner"
%option prefix="mathy_"

NEWLINE     \n
FORALL      forall
SIGMA       sigma
WHERE       where
PRODUCT     product
SQRT        sqrt
IDENTIFIER  [a-zA-Z_]([a-zA-Z0-9_])*
LEFTPAR     \(
RIGHTPAR    \)
LEFTCURLY   \{
RIGHTCURLY  \}
LEFTSQR     \[
RIGHTSQR    \]
EQUALS      =
WHITESPACE  [ \t\v\f]
INTCONST    [0-9]+
FLOATCONST  [1-9][0-9]*(\.[0-9]+)?|0(\.[0-9]+)?
COMPARISON  (<|<=)
OPERATOR    (\+|\-|\/|\*|%)
WILDCARD    .

%%

{NEWLINE}       {
                    return mathy::Parser::make_NEWLINE(yytext[0], mathy::location()); 
                }

{FORALL}        {
                    return mathy::Parser::make_FORALL(yytext, mathy::location()); 
                }

{SIGMA}         {
                    return mathy::Parser::make_SIGMA(yytext, mathy::location()); 
                }

{WHERE}         {
                    return mathy::Parser::make_WHERE(yytext, mathy::location()); 
                }

{PRODUCT}       {
                    return mathy::Parser::make_PRODUCT(yytext, mathy::location()); 
                }

{SQRT}          {
                    return mathy::Parser::make_SQRT(yytext, mathy::location()); 
                }

{IDENTIFIER}    {
                    return mathy::Parser::make_IDENTIFIER(yytext, mathy::location()); 
                }
            
{LEFTPAR}       { 
                    return mathy::Parser::make_LEFTPAR(mathy::location());
                }
            
{RIGHTPAR}      { 
                    return mathy::Parser::make_RIGHTPAR(mathy::location());
                }

{LEFTCURLY}     {
                    return mathy::Parser::make_LEFTCURLY(mathy::location());
                }
            
{RIGHTCURLY}    { 
                    return mathy::Parser::make_RIGHTCURLY(mathy::location());
                }

{LEFTSQR}       {
                    return mathy::Parser::make_LEFTSQR(mathy::location());
                }
            
{RIGHTSQR}      { 
                    return mathy::Parser::make_RIGHTSQR(mathy::location());
                }

{EQUALS}        {
                    return mathy::Parser::make_EQUALS(yytext, mathy::location());
                }
            
{WHITESPACE}    {    }

{INTCONST}      {
                    uint64_t number = strtoull(yytext, 0, 10);
                    return mathy::Parser::make_INTCONST(number, mathy::location());
                }

{FLOATCONST}    {
                    double number = stod(yytext);
                    return mathy::Parser::make_FLOATCONST(number, mathy::location());
                }

{COMPARISON}    {
                    return mathy::Parser::make_COMPARISON(yytext, mathy::location());
                }

{OPERATOR}      {
                    return mathy::Parser::make_OPERATOR(yytext, mathy::location());
                }

{WILDCARD}      {   }
            
<<EOF>>         { return mathy::Parser::make_END(mathy::location()); }


%%
