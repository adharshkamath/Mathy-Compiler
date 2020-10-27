#ifndef FORALL_H
#define FORALL_H

#include <iostream>
#include <fstream>
#include <variant>
#include "gen_node.h"

namespace mathy {
    class SigmaProd;

    class ForAll : public GeneralNode {
    public:
        std::variant<GeneralNode *, ForAll *, SigmaProd *, long int> child;
        std::string loop_var;

        ForAll() : GeneralNode(FORALL_NODE), child(NULL) {
        }

        ForAll(const Bound &bound, const std::variant<GeneralNode *, ForAll *, SigmaProd *, long int> nest,
               const std::string &id) : GeneralNode(FORALL_NODE), child(NULL) {
            this->node_type = FORALL_NODE;
            this->loop_var = id;
            this->gen_bound = bound;
            switch (nest.index()) {
                case 0:
                    this->child = std::get<0>(nest);
                    break;
                case 1:
                    this->child = std::get<1>(nest);
                    break;
                case 2:
                    this->child = std::get<2>(nest);
                    break;
                default:
                    break;
            }
        }

        explicit ForAll(const Bound &bound) : GeneralNode(FORALL_NODE), child(NULL) {
            this->node_type = FORALL_NODE;
            this->gen_bound = bound;
        }

        void set_child(GeneralNode &nest);

        void set_child(ForAll &nest);

        void set_child(SigmaProd &nest);

        void gen_code(std::fstream &file);
    };

} // namespace mathy

#endif