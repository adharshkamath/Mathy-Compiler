/*
    This file contains the declatasion of SigmaProd and 
    definitions of some of the methods of the SigmaProd class.
    which can be used to represent a Sigma stamenent like
        a = Σ(b[i]) | 0<i<100
    or a Product statement like
        p = Π(q[j]) | 10<=j<=120
    You can also replace the unicode characters with `sigma` for Σ,
    `product` for Π, and `where` for |.
*/

#ifndef SIGMA_PROD_H
#define SIGMA_PROD_H

#include <string>
#include <fstream>
#include <variant>
#include "gen_node.h"
#include "forall.h"

namespace mathy {

    class SigmaProd : public GeneralNode {
    public:
        std::string LHS, RHS;

        SigmaProd() : GeneralNode(SIGMA_NODE), LHS(""), RHS("") {}

        SigmaProd(Bound &bound, int type, std::variant<GeneralNode, ForAll, SigmaProd, long int> rhs) :
                GeneralNode(type, bound) {
            auto rhs_str = std::get_if<GeneralNode>(&rhs);
            this->RHS = rhs_str->expression;
        }

        SigmaProd(Bound &bound, int type, const std::string &rhs, const std::string &lhs) :
                GeneralNode(type, bound) {
            this->RHS = rhs;
            this->LHS = lhs;
        }

        SigmaProd(Bound &bound, int type, const std::string &rhs) :
                GeneralNode(type, bound) {
            this->RHS = rhs;
        }

        void set_lhs(const std::string &lhs);

        void set_rhs(const std::string &rhs);

        void set_type(int type);

        void gen_code(std::fstream &file);
    };
} // namespace mathy

#endif