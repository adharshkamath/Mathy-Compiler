#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include "forall.h"
#include "sigma_prod.h"

namespace mathy {

    extern std::string data_type;

    extern GeneralNode *gen_ptr;
    extern SigmaProd *sp_ptr;
    extern ForAll *for_ptr;
    extern std::variant<GeneralNode *, ForAll *, SigmaProd *, long int> current_stmt;
    extern std::variant<GeneralNode *, ForAll *, SigmaProd *, long int> temp_stmt;
    extern std::variant<GeneralNode *, ForAll *, SigmaProd *, long int> root;
    extern std::variant<GeneralNode, ForAll, SigmaProd, long int> current_node;

    int newVariable(const std::string &identifier);

    int newBound(const std::string &identifier, const std::string &low, const std::string &high);

    int addArrDimension(const std::string &identifier, std::string &bound);

    int addArrDimension_util(const std::string &identifier, const std::string &bound);

    bool isVariableDeclared(const std::string &identifier);

    bool isBoundDeclared(const std::string &identifier);

    void splitTerms(const std::string &str, const std::regex &reg, std::vector <std::string> &res);

    void printStuff();

    void initOutput();

    bool isNumber(const std::string &s);

    bool isString(const std::string &s);

    bool isVariableFinalized(const std::string &identifier);

    int finalizeVariable(const std::string &identifier);

    void declareVars(std::fstream &output);

    void traverse(GeneralNode *genp);

    void traverse(ForAll *genp);

    void traverse(SigmaProd *genp);

    void freeBound(const std::string &identifier);

} // namespace mathy

#endif