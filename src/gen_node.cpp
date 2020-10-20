#include <iostream>
#include <string>
#include <fstream>
#include "bounds.h"
#include "forall.h"
#include "sigma_prod.h"
#include "gen_node.h"

using namespace mathy;

void GeneralNode::make_parent() {
    this->parent = true;
}

void GeneralNode::gen_code(std::ofstream &m_file) {
    if (this->node_type == EXPRN_NODE) {
        m_file << this->expression << std::endl;
        if(this->next.index() == 0) {
            auto temp = std::get<0>(this->next);
            temp->gen_code(m_file);
        }
        else if(this->next.index() == 1) {
            auto temp = std::get<1>(this->next);
            temp->gen_code(m_file);
        }
        else if(this->next.index() == 2) {
            auto temp = std::get<2>(this->next);
            temp->gen_code(m_file);
        }
    }
    std::cout << "ERROR: Tried to generate code from a non espression general node!" << std::endl;
}