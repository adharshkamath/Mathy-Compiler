/*
    This file contains the declaration of the GeneralNode class
    and definitions of some of its methods. This class is the parent class of 
    ForAll, SigmaProd classes. This class can be used to represent 
    simple exprssions like
        a = 100
*/

#ifndef GEN_NODE_H
#define GEN_NODE_H

#include <iostream>
#include <string>
#include <fstream>
#include <variant>
#include "bounds.h"

#define EXPRN_NODE 0
#define SIGMA_NODE 1
#define PRODUCT_NODE 2
#define FORALL_NODE 3
#define SQRT_NODE  4

namespace mathy {


    class GeneralNode {
    public:
        bool parent;
        int node_type;
        Bound gen_bound;
        std::string expression;
        std::variant<GeneralNode *, ForAll *, SigmaProd *, long int> next;

        GeneralNode() : parent(true), node_type(EXPRN_NODE), gen_bound(), expression(""), next(NULL) {}

        void make_parent();

        void gen_code(std::fstream &m_file);

        explicit GeneralNode(int type) : parent(true), node_type(type), gen_bound(), expression(""),
                                         next(NULL) {}

        GeneralNode(int type, const std::string &exprn) : parent(true), node_type(type), gen_bound(),
                                                          expression(exprn), next(NULL) {}

        GeneralNode(int type, Bound &bound) : parent(true), node_type(type), gen_bound(bound), expression(""),
                                              next(NULL) {}
    };
} // namespace mathy

#endif