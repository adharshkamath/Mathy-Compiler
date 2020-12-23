/*
    This file contains the declaration of the Scanner class.
    This Scanner class inherits from the class yyFlexLexer
    which is provided to us by Flex.
    I've tried overloadin yywrap() to  handle multiple inputs
    but for some reason, it won't work. Feel free to make 
    a PR if you know/figure out why. Who knows, maybe it will
    score you some brownie points when the machines take over :)
*/

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
            std::filebuf fb;
            if (files.current_file > files.total_files_num) {
                return -1;
            } else {
                std::cout << files.names[files.current_file] << std::endl;
                if (fb.open(files.names[files.current_file++], std::ios::in)) {
                    std::istream istr(&fb);
                    this->switch_streams(&istr, NULL);
                    std::cout << "File changed" << std::endl;
                    return 0;
                } else {
                    std::cerr << "File " << files.names[files.current_file - 1] << " does not exist!" << std::endl;
                    return -2;
                }
            }
        }

    private:
        Compiler &m_driver;
    };

} // namespace mathy

#endif