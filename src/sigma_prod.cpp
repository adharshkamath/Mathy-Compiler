#include <string>
#include <fstream>
#include "gen_node.h"
#include "sigma_prod.h"
#include "forall.h"

using namespace mathy;

void SigmaProd::set_lhs(const std::string &lhs) {
    this->LHS = lhs;
}

void SigmaProd::set_rhs(const std::string &rhs) {
    this->RHS = rhs;
}

void SigmaProd::set_type(int type) {
    this->node_type = type;
}

void SigmaProd::gen_code(std::ofstream &m_file) {
    if(this->gen_bound.lower.index() == 0) {
        auto low = std::get<0>(this->gen_bound.lower);
        m_file << "for(int " << this->gen_bound.identifier << " = " << low << "; " << this->gen_bound.identifier ;
    }
    else {
        auto low = std::get<1>(this->gen_bound.lower);
        m_file << "for(int " << this->gen_bound.identifier << " = " << low << "; " << this->gen_bound.identifier ;
    }
    if(this->gen_bound.upper.index() == 0) {
        auto up = std::get<0>(this->gen_bound.lower);
        m_file << "<= " << up << "; " << this->gen_bound.identifier << "++) {" << std::endl;
    }
    else {
        auto up = std::get<1>(this->gen_bound.lower);
        m_file << "<= " << up << "; " << this->gen_bound.identifier << "++) {" << std::endl;
    }
    std::string op = this->node_type == SIGMA_NODE ? " += " : " *= ";
    m_file << this->LHS << op << this->RHS << std::endl;
    m_file << "} " << std::endl;
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