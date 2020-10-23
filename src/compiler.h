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

        void clear();

        void changeInput(std::istream *is);

        void increaseLocation(unsigned int loc);

        unsigned int location() const;

        Scanner m_scanner;
        Parser m_parser;
        unsigned int m_location;
    };

} // namespace mathy

#endif
