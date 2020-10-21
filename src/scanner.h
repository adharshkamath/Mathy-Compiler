#ifndef SCANNER_H
#define SCANNER_H

#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer mathy_FlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL mathy::Parser::symbol_type mathy::Scanner::get_next_token()

#include "parser.hpp"

namespace mathy {

class Compiler;
class Parser;
class Scanner : public yyFlexLexer {
public:
    Scanner(Compiler &driver) : m_driver(driver) {}
	virtual ~Scanner() {}
	virtual mathy::Parser::symbol_type get_next_token();
    int yywrap() {
        return 1;
    }

private:
    Compiler &m_driver;
};

}

#endif