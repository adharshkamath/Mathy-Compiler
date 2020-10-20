#include <fstream>
#include "gen_node.h"
#include "forall.h"
#include "sigma_prod.h"

using namespace mathy;

void ForAll::set_child(GeneralNode &nest) {
    this->child = &nest;
}

void ForAll::set_child(ForAll &nest) {
    this->child = &nest;
}

void ForAll::set_child(SigmaProd &nest) {
    this->child = &nest;
}

void ForAll::gen_code(std::ofstream &m_file) {
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
    m_file << this->expression << std::endl;
        if(this->child.index() == 0) {
            auto temp = std::get<0>(this->child);
            temp->gen_code(m_file);
        }
        else if(this->child.index() == 1) {
            auto temp = std::get<1>(this->child);
            temp->gen_code(m_file);
        }
        else if(this->child.index() == 2) {
            auto temp = std::get<2>(this->child);
            temp->gen_code(m_file);
        }
    m_file << "} " << std::endl;
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