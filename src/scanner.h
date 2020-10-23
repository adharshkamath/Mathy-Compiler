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
            std::cout << "yywrap called" << std::endl;
            std::filebuf fb;
            if (files.current_file > files.total_files_num) {
                std::cout << "All files done!" << std::endl;
                return -1;
            } else {
                std::cout << files.names[files.current_file] << std::endl;
                if (fb.open(files.names[files.current_file++], std::ios::in)) {
                    std::istream istr(&fb);
                    this->switch_streams(&istr, NULL);
                    std::cout << "File changed" << std::endl;
                    return 0;
                }
                else {
                    std::cerr << "File " << files.names[files.current_file -1] << " does not exist!" << std::endl;
                    return -2;
                }
            }
        }

    private:
        Compiler &m_driver;
    };

} // namespace mathy

#endif