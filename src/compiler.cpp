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

using namespace mathy;

Compiler::Compiler() :
        m_scanner(*this),
        m_parser(m_scanner, *this),
        m_location(0) {}

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
    return 1;
}