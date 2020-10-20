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

%option nodefault yylineno noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="mathy_"

%%

\n              { 
                    cout << "Scanner: New Line " << endl;
                    return mathy::Parser::make_NEWLINE(yytext[0], mathy::location()); 
                }

forall          {
                    cout << "Scanner: For all statement [" << yytext << "]" << endl;
                    return mathy::Parser::make_FORALL(yytext, mathy::location()); 
                }

sigma          {
                    cout << "Scanner: Sigma statement [" << yytext << "]" << endl;
                    return mathy::Parser::make_SIGMA(yytext, mathy::location()); 
                }


where          {
                    cout << "Scanner: Where statement [" << yytext << "]" << endl;
                    return mathy::Parser::make_WHERE(yytext, mathy::location()); 
                }

product          {
                    cout << "Scanner: Product statement [" << yytext << "]" << endl;
                    return mathy::Parser::make_PRODUCT(yytext, mathy::location()); 
                }

[a-zA-Z_]([a-zA-Z0-9_])*    {
                    cout << "Scanner: identifier [" << yytext << "]" << endl;
                    return mathy::Parser::make_IDENTIFIER(yytext, mathy::location()); 
                }
            
\(          {
                cout << "Scanner: '('" << endl;
                return mathy::Parser::make_LEFTPAR(mathy::location());
            }
            
\)          { 
                cout << "Scanner: ')'" << endl;
                return mathy::Parser::make_RIGHTPAR(mathy::location());
            }

\{          {
                cout << "Scanner: '{'" << endl;
                return mathy::Parser::make_LEFTCURLY(mathy::location());
            }
            
\}          { 
                cout << "Scanner: '}'" << endl;
                return mathy::Parser::make_RIGHTCURLY(mathy::location());
            }

\[          {
                cout << "Scanner: '['" << endl;
                return mathy::Parser::make_LEFTSQR(mathy::location());
            }
            
\]         { 
                cout << "Scanner: ']'" << endl;
                return mathy::Parser::make_RIGHTSQR(mathy::location());
            }

;           {
                cout << "Scanner: ';'" << endl;
                return mathy::Parser::make_SEMICOLON(mathy::location());
            }

=           {
                cout << "Scanner: '='" << endl;
                return mathy::Parser::make_EQUALS(yytext, mathy::location());
            }
            
,           {
                cout << "Scanner: ','" << endl;
                return mathy::Parser::make_COMMA(mathy::location());
            }
            
[ \t\v\f]   {
                cout << "Scanner: whitechar (ignored)" << endl;
            }

[1-9][0-9]* {
                cout << "Scanner: Decimal number: " << yytext << endl;
                uint64_t number = strtoull(yytext, 0, 10);
                return mathy::Parser::make_INTCONST(number, mathy::location());
            }

[1-9][0-9]*(\.[0-9]+)?|0(\.[0-9]+)? {
                cout << "Scanner: Double: " << yytext << endl;
                double number = stod(yytext);
                return mathy::Parser::make_FLOATCONST(number, mathy::location());
            }

(<|<=)        {
                cout << "Scanner: Comparison Op : " << yytext << endl;
                return mathy::Parser::make_COMPARISON(yytext, mathy::location());
            }

(\+|\-|\/|\*|%) {
                    cout << "Scanner: Normal Op : " << yytext << endl;
                    return mathy::Parser::make_OPERATOR(yytext, mathy::location());
                }

.           { 
                cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return mathy::Parser::make_END(mathy::location()); }


%%
