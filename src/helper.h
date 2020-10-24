#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace mathy {

    int newVariable(const std::string &identifier);

    int newBound(const std::string &identifier, const std::string& low, const std::string& high);

    int addArrDimension(const std::string &identifier, int dimension);

    int addArrDimension(const std::string &identifier, const std::string &bound);

    bool isVariableDeclared(const std::string &identifier);

    bool isBoundDeclared(const std::string &identifier);

    void printStuff();

    void initOutput();

} // namespace mathy

#endif