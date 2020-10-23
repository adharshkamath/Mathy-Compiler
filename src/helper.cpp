#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "helper.h"

namespace mathy {
    std::unordered_map <std::string, std::pair<int, int>> bounds_table;
    std::unordered_map <std::string, std::vector<int>> variable_table;

    int newVariable(const std::string &identifier) {
        if (variable_table.find(identifier) != variable_table.end()) {
            return -1;
        }
        variable_table[identifier] = std::vector<int>();
        return 0;
    }

    int newBound(const std::string &identifier, int low, int high) {
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
        variable_table[identifier].push_back(bounds_table[bound].second + 1);
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
        for(auto x : variable_table) {
            std::cout << x.first << std::endl;
        }
        for(auto x : bounds_table) {
            std::cout << x.first << std::endl;
        }
        std::cout << "------ Final stuff ------" << std::endl;
    }
} // namespace mathy

using namespace mathy;
