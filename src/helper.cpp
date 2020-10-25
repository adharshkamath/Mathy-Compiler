#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <variant>
#include <regex>
#include <algorithm>
#include "helper.h"

namespace mathy {
    std::unordered_map <std::string, std::pair<std::string, std::string>> bounds_table;
    std::unordered_map <std::string, std::vector<std::string> > variable_table;

    int newVariable(const std::string &identifier) {
        if (variable_table.find(identifier) != variable_table.end()) {
            return -1;
        }
        if (bounds_table.find(identifier) != bounds_table.end()) {
            return -2;
        }
        variable_table[identifier] = std::vector<std::string>();
        return 0;
    }

    int newBound(const std::string &identifier, const std::string &low, const std::string &high) {
        int ret = 0;
        if (bounds_table.find(identifier) != bounds_table.end()) {
            std::cout << "Warning: Bound variable re-used!" << std::endl;
            ret = -1;
        }
        bounds_table[identifier].first = low;
        bounds_table[identifier].second = high;
        return ret;
    }

    std::string getBoundValue(const std::string& identifier) {
        if (bounds_table.find(identifier) != bounds_table.end()) {
            if(isNumber(bounds_table[identifier].second))
            return bounds_table[identifier].second;
            else
            {
                std::string bound = bounds_table[identifier].second;
                std::string res = "";
                std::vector<std::string> ops;
                std::regex operands("[^+-/*]+");
                splitTerms(bound, operands, ops);
                int size = ops.size();
                int location = -1;
                for(int i=0; i<size; i++) {
                    location += ops[i].length();
                    if(isNumber(ops[i])) {
                        res += ops[i];
                        if(location+1 < bound.length()) {
                            res += bound[location+1];
                            location++;
                        }
                    }
                    else {
                        std::string temp = getBoundValue(ops[i]);
                        if(temp.length() == 0) {
                            res += ops[i];
                            if(location+1 < bound.length()) {
                                res += bound[location+1];
                                location++;
                            }
                        }
                        else {
                            res += temp;
                            if(location+1 < bound.length()) {
                                res += bound[location+1];
                                location++;
                            }
                        }
                    }
                }
                return res;
            }
            
        }
        return "";
    }

    int addArrDimension(const std::string &identifier, std::string &bound) {
        if(!isVariableDeclared(identifier)) return -1;
        std::vector<std::string> ops;
        std::replace(bound.begin(), bound.end(), ']', '_');
        std::replace(bound.begin(), bound.end(), '[', '_');
        std::regex operands("[^_]+");
        splitTerms(bound, operands, ops);
        int size = ops.size();
        for(int i=0; i<size; i++) {
            addArrDimension_util(identifier, ops[i]);
        }
        return 0;
    }

    int addArrDimension_util(const std::string &identifier, const std::string &bound) {
        std::string res = "";
        std::vector<std::string> ops;
        std::regex operands("[^+-/*]+");
        splitTerms(bound, operands, ops);
        int size = ops.size();
        int location = -1;
        for(int i=0; i<size; i++) {
            location += ops[i].length();
            if(isNumber(ops[i])) {
                res += ops[i];
                if(location+1 < bound.length()) {
                    char op = bound[location+1];
                    res += op;
                }
            }
            else {
                std::string temp = getBoundValue(ops[i]);
                if(temp.length() == 0) {
                    res += ops[i];
                    if(location+1 < bound.length())
                    res += bound[location+1];
                }
                else {
                    res += temp;
                    if(location+1 < bound.length())
                    res += bound[location+1];
                }
            }
        }
        variable_table[identifier].push_back(res);
        return 0;
    }

    bool isVariableFinalized(const std::string& identifier) {
        if(!isVariableDeclared(identifier)) {
            return false;
        }
        else {
            return !(std::any_of(variable_table[identifier].begin(), variable_table[identifier].end(), isString));
        }
    }

    int finalizeVariable(const std::string& identifier) {
        bool check = std::none_of(variable_table[identifier].begin(), variable_table[identifier].end(), isString);
        if(check) return 1;
        else {
            for(int j=0; j < variable_table[identifier].size(); j++) {
                std::string res = "";
                std::vector<std::string> ops;
                auto bound = variable_table[identifier][j];
                if(isNumber(bound)) continue;
                std::regex operands("[^+-/*]+");
                splitTerms(bound, operands, ops);
                int size = ops.size();
                int location = -1;
                for(int i=0; i<size; i++) {
                    location += ops[i].length();
                    if(isNumber(ops[i])) {
                        res += ops[i];
                        if(location+1 < bound.length())
                        {
                            res += bound[location+1];
                            location++;
                        }
                    }
                    else {
                        std::string temp = getBoundValue(ops[i]);
                        if(temp.length() == 0) {
                            res += ops[i];
                            if(location+1 < bound.length())
                            {
                                res += bound[location+1];
                                location++;
                            }
                        }
                        else {
                            res += temp;
                            if(location+1 < bound.length())
                            {
                                res += bound[location+1];
                                location++;
                            }
                        }
                    }
                }
                variable_table[identifier][j] = res;
            }
        }
        return 0;
    }

    bool isVariableDeclared(const std::string &identifier) {
        return variable_table.find(identifier) != variable_table.end();
    }

    bool isBoundDeclared(const std::string &identifier) {
        return bounds_table.find(identifier) != bounds_table.end();
    }

    void printStuff() {
        std::cout << "------ Final stuff ------" << std::endl;
        for (auto x : variable_table) {
            std::cout << x.first << " - ";
            for(auto y : x.second) {
                std::cout << y << " ";
            }
            std::cout << std::endl;
        }
        for (auto x : bounds_table) {
            std::cout << x.first << " - " << x.second.first << ":" << x.second.second << std::endl;
        }
        std::cout << "------ Final stuff ------" << std::endl;
    }

    void initOutput() {
        std::fstream output;
        output.open("output.c");
        output << "#include <stdio.h>\n#include <stdlib.h>\n#include <omp.h>\n" \
                    "\nint main() {" << std::endl;

        // Rest of the important stuff - Function call 

        output << std::endl << "return 0;" << std::endl << "}" << std::endl;
    }

    void splitTerms(const std::string &str, const std::regex &reg, std::vector <std::string> &res) {
        auto words_begin = std::sregex_iterator(str.begin(), str.end(), reg);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            res.push_back((*i).str());
    }

    bool isNumber(const std::string& s) {
        if(s.empty() || std::isspace(s[0]) || std::isalpha(s[0])) 
            return false ;
        char * p ;
        strtod(s.c_str(), &p) ;
        return (*p == 0) ;
    }

    bool isString(const std::string& s) {
        return !isNumber(s);
    }

} // namespace mathy

using namespace mathy;
