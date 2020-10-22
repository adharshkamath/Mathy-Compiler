#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "scanner.h"
#include "parser.hpp"
#include "forall.h"
#include "sigma_prod.h"
#include "gen_node.h"

#ifndef COMPILER_H
#define COMPILER_H

namespace mathy
{
    class Scanner;
    class Parser;

    int newVariable(std::string identifier);

    int newBound(std::string identifier, int low, int high);

    int addArrDimension(std::string identifier, int dimension);

    int addArrDimension(std::string identifier, std::string bound);

    bool isVariableDeclared(std::string identifier);

    bool isBoundDeclared(std::string identifier);

    class Compiler
    {
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
