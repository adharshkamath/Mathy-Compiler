#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>

namespace mathy {

    extern std::string data_type;

    int newVariable(const std::string &identifier);

    int newBound(const std::string &identifier, const std::string &low, const std::string &high);

    int addArrDimension(const std::string &identifier, std::string &bound);
    
    int addArrDimension_util(const std::string &identifier, const std::string &bound);

    bool isVariableDeclared(const std::string &identifier);

    bool isBoundDeclared(const std::string &identifier);

    void splitTerms(const std::string &str, const std::regex &reg, std::vector <std::string> &res);

    void printStuff();

    void initOutput();

    bool isNumber(const std::string& s);

    bool isString(const std::string& s);

    bool isVariableFinalized(const std::string& identifier);

    int finalizeVariable(const std::string& identifier);

    void declareVars(std::fstream& output);

} // namespace mathy

#endif