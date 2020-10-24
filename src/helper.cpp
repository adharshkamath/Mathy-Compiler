#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <variant>
#include "helper.h"

namespace mathy {
    std::unordered_map <std::string, std::pair<std::string, std::string>> bounds_table;
    std::unordered_map <std::string, std::vector<int>> variable_table;

    int newVariable(const std::string &identifier) {
        if (variable_table.find(identifier) != variable_table.end()) {
            return -1;
        }
        if(bounds_table.find(identifier) != bounds_table.end()) {
            return -2;
        }
        variable_table[identifier] = std::vector<int>();
        return 0;
    }

    int newBound(const std::string &identifier, const std::string& low, const std::string& high) {
        int ret = 0;
        if (bounds_table.find(identifier) != bounds_table.end()) {
            std::cout << "Warning: Bound variable re-used!" << std::endl;
            ret = -1;
        }
        bounds_table[identifier].first = low;
        bounds_table[identifier].second = high;
        return ret;
    }

    int addArrDimension(const std::string &identifier, int dims) {
        variable_table[identifier].push_back(dims);
        return 0;
    }

    int addArrDimension(const std::string &identifier, const std::string &bound) {
        if (bounds_table.find(bound) == bounds_table.end()) {
            return -1;
        }
        std::string b = bounds_table[bound].second;
        // variable_table[identifier].push_back();
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
            std::cout << x.first << std::endl;
        }
        for (auto x : bounds_table) {
            std::cout << x.first << " - " << x.second.first << ":"  << x.second.second << std::endl;
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
} // namespace mathy

using namespace mathy;
