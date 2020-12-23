/*
    This file contains the declaration and definition of the class Bound 
    used to represent the bounds of a variable in a statement
*/


#ifndef BOUNDS_H
#define BOUNDS_H

#include <string>
#include <variant>

namespace mathy {
    class ForAll;

    class SigmaProd;

    class Bound {
    public:
        std::string lower, upper;   // Upper and Lower bounds of the bound variable 
        std::string identifier; 

        Bound(std::string &low, std::string &lower_comp, const std::string &id,
              std::string &higher_comp, std::string &high) {
            this->identifier = id;
            if (lower_comp.compare("<=")) {
                low.append("+1");
            }
            this->lower = low;
            if (higher_comp.compare("<=")) {
                high.append("-1");
            }
            this->upper = high;
        }

        Bound() {}
    };
} // namespace mathy
#endif