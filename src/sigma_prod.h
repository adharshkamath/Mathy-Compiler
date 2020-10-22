#include <string>
#include <fstream>
#include <variant>
#include "gen_node.h"
#include "forall.h"

#ifndef SIGMA_PROD_H
#define SIGMA_PROD_H

namespace mathy {

    class SigmaProd : public GeneralNode {
    public:
        std::string LHS, RHS;

        SigmaProd() : GeneralNode(SIGMA_NODE), LHS(""), RHS("") {}

        SigmaProd(Bound &bound, int type, std::variant <GeneralNode, ForAll, SigmaProd> rhs) :
                GeneralNode(type, bound) {
            auto rhs_str = std::get_if<GeneralNode>(&rhs);
            this->RHS = rhs_str->expression;
        }

        SigmaProd(Bound &bound, int type, const std::string &rhs) :
                GeneralNode(type, bound) {
            this->RHS = rhs;
        }

        void set_lhs(const std::string &lhs);

        void set_rhs(const std::string &rhs);

        void set_type(int type);

        void gen_code(std::ofstream &file);
    };
} // namespace mathy

#endif