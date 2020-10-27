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

void ForAll::gen_code(std::fstream &m_file) {
    m_file << "for(int " << this->gen_bound.identifier << " = " << this->gen_bound.lower << "; "
           << this->gen_bound.identifier;
    m_file << "<= " << this->gen_bound.upper << "; " << this->gen_bound.identifier << "++) {" << std::endl;
    m_file << this->expression << std::endl;
    int tmp = this->child.index();
    if (tmp == 0) {
        std::cout << "For node ------- |" << this->expression << "|" << std::endl;
        auto temp = std::get<0>(this->child);
        temp->gen_code(m_file);
    } else if (tmp == 1) {
        auto temp = std::get<1>(this->child);
        temp->gen_code(m_file);
    } else if (tmp == 2) {
        auto temp = std::get<2>(this->child);
        temp->gen_code(m_file);
    }
    m_file << "} " << std::endl;
    if (this->next.index() == 0) {
        auto temp = std::get<0>(this->next);
        temp->gen_code(m_file);
    } else if (this->next.index() == 1) {
        auto temp = std::get<1>(this->next);
        temp->gen_code(m_file);
    } else if (this->next.index() == 2) {
        auto temp = std::get<2>(this->next);
        temp->gen_code(m_file);
    } else if (this->next.index() == 3) {
        return;
    }
}