#include "compiler.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "forall.h"
#include "sigma_prod.h"
#include "gen_node.h"
#include "scanner.h"
#include "parser.hpp"
#include "files.h"

namespace mathy
{
    std::unordered_map<std::string, std::pair<int, int>> bounds_table;
    std::unordered_map<std::string, std::vector<int>> variable_table;
}

using namespace mathy;

int newVariable(const std::string& ident)
{
    if (variable_table.find(ident) != variable_table.end())
    {
        return -1;
    }
    variable_table[ident] = std::vector<int>();
    return 0;
}

int newBound(const std::string& ident, int low, int high)
{
    int ret = 0;
    if (bounds_table.find(ident) != bounds_table.end())
    {
        std::cout << "Warning: Bound variable re-used!" << std::endl;
        ret = -1;
    }
    bounds_table[ident].first = low;
    bounds_table[ident].second = high;
    return ret;
}

int addArrDimension(const std::string& ident, int dims)
{
    variable_table[ident].push_back(dims);
    return 0;
}

int addArrDimension(const std::string& ident, const std::string& bound)
{
    if (bounds_table.find(bound) == bounds_table.end())
    {
        return -1;
    }
    variable_table[ident].push_back(bounds_table[bound].second + 1);
    return 0;
}

bool isVariableDeclared(const std::string& identifier)
{
    return variable_table.find(identifier) != variable_table.end();
}

bool isBoundDeclared(const std::string& identifier)
{
    return bounds_table.find(identifier) != bounds_table.end();
}

Compiler::Compiler() :
    m_scanner(*this),
    m_parser(m_scanner, *this),
    m_location(0) { }

int Compiler::parse() {
    m_location = 0;
    return m_parser.parse();
}

void Compiler::clear() {
    m_location = 0;
}

void Compiler::changeInput(std::istream *is) {
    m_scanner.switch_streams(is, NULL);
}

void Compiler::increaseLocation(unsigned int loc) {
    m_location += loc;
}

unsigned int Compiler::location() const {
    return m_location;
}

int mathy_FlexLexer::yywrap() {
    std::cout << "----- Original yywrap() called -----" << std::endl;
    std::cout << filenum << std::endl;
    return 1;
}