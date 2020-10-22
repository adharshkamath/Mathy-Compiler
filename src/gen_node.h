#include <iostream>
#include <string>
#include <fstream>
#include <variant>
#include "bounds.h"

#ifndef GEN_NODE_H
#define GEN_NODE_H

#define EXPRN_NODE 0
#define SIGMA_NODE 1
#define PRODUCT_NODE 2
#define FORALL_NODE 3

namespace mathy {


    class GeneralNode {
    public:
        bool parent;
        int node_type;
        Bound gen_bound;
        std::string expression;
        std::variant<GeneralNode *, ForAll *, SigmaProd *> next;

        GeneralNode() : parent(false), node_type(EXPRN_NODE), gen_bound(), expression("") {}

        void make_parent();

        void gen_code(std::ofstream &m_file);

        explicit GeneralNode(int type) : parent(false), node_type(type), gen_bound(), expression("") {}

        GeneralNode(int type, const std::string &exprn) : parent(false), node_type(type), gen_bound(),
                                                          expression(exprn) {}

        GeneralNode(int type, Bound &bound) : parent(false), node_type(type), gen_bound(bound), expression("") {}
    };
} // namespace mathy

#endif