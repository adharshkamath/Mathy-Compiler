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

%%

\n              {
                    
                    return mathy::Parser::make_NEWLINE(yytext[0], mathy::location()); 
                }

forall          {
                    
                    return mathy::Parser::make_FORALL(yytext, mathy::location()); 
                }

sigma          {
                    
                    return mathy::Parser::make_SIGMA(yytext, mathy::location()); 
                }


where          {
                    
                    return mathy::Parser::make_WHERE(yytext, mathy::location()); 
                }

product          {
                    
                    return mathy::Parser::make_PRODUCT(yytext, mathy::location()); 
                }

[a-zA-Z_]([a-zA-Z0-9_])*    {
                                
                                return mathy::Parser::make_IDENTIFIER(yytext, mathy::location()); 
                            }
            
\(          { 
                
                return mathy::Parser::make_LEFTPAR(mathy::location());
            }
            
\)          { 
                
                return mathy::Parser::make_RIGHTPAR(mathy::location());
            }

\{          {
                
                return mathy::Parser::make_LEFTCURLY(mathy::location());
            }
            
\}          { 
                
                return mathy::Parser::make_RIGHTCURLY(mathy::location());
            }

\[          {
                
                return mathy::Parser::make_LEFTSQR(mathy::location());
            }
            
\]         { 
                
                return mathy::Parser::make_RIGHTSQR(mathy::location());
            }

;           {
                
                return mathy::Parser::make_SEMICOLON(mathy::location());
            }

=           {
                
                return mathy::Parser::make_EQUALS(yytext, mathy::location());
            }
            
,           {
                
                return mathy::Parser::make_COMMA(mathy::location());
            }
            
[ \t\v\f]   {
                
                }

[0-9]+ {
                
                uint64_t number = strtoull(yytext, 0, 10);
                return mathy::Parser::make_INTCONST(number, mathy::location());
            }

[1-9][0-9]*(\.[0-9]+)?|0(\.[0-9]+)? {
                
                double number = stod(yytext);
                return mathy::Parser::make_FLOATCONST(number, mathy::location());
            }

(<|<=)        {
                
                return mathy::Parser::make_COMPARISON(yytext, mathy::location());
            }

(\+|\-|\/|\*|%) {
                
                    return mathy::Parser::make_OPERATOR(yytext, mathy::location());
                }

.           { 
                
                }
            
<<EOF>>     { return mathy::Parser::make_END(mathy::location()); }


%%
