#ifndef SCANNER_H
#define SCANNER_H

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer mathy_FlexLexer

#include <FlexLexer.h>

#endif

#undef YY_DECL
#define YY_DECL mathy::Parser::symbol_type mathy::Scanner::get_next_token()

#include "parser.hpp"
#include "files.h"

namespace mathy {

    class Compiler;

    class Parser;

    class Scanner : public yyFlexLexer {
    public:
        Scanner(Compiler &driver) : m_driver(driver) {}
        Files files;
        virtual ~Scanner() {}

        virtual mathy::Parser::symbol_type get_next_token();

        int yywrap() {
            std::string filename;
            if (files.next(&filename) == -1) {
                return 1;
            } else {
                std::filebuf fb;
                if (fb.open(filename, std::ios::in)) {
                    std::istream istr(&fb);
                    this->switch_streams(&istr, NULL);
                }
                std::cout << "----- Scanner yywrap() called -----" << std::endl;
                return 0;
            }
        }

    private:
        Compiler &m_driver;
    };

} // namespace mathy

#endif