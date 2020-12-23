/*
    This file contains the declaration of the class Compiler 
    which is instantiated in main(), to scan and parse the input.
    The Compiler class contains a Scanner (Lexer) and a Parser.
*/


#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "scanner.h"
#include "parser.hpp"
#include "forall.h"
#include "sigma_prod.h"
#include "gen_node.h"

namespace mathy {
    class Scanner;

    class Parser;

    class Compiler {
    public:
        Compiler();

        int parse();

        void changeInput(std::istream *is);             // To be used in future for multiple file input support

        void increaseLocation(unsigned int loc);        // To be used in future for improved error messages

        unsigned int location() const;

        Scanner m_scanner;
        Parser m_parser;
        unsigned int m_location;
    };
} // namespace mathy
#endif
