/*
    This file contains the definition of some of the methods of
    the class ForAll which represents a Forall statement
    Eg., 
        ∀(i) | 0<i<100 {
            a[i] = i+1
        }
    You can also replace the unicode characters with `forall` for ∀,
    and `where` for |.
*/

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

/*
    This is the important funtion that writes to the output file.
    For every forall node in the 'AST' (It's not a sophisticated syntax tree, hence the quotes)
    this function generates a for loop accordingly
*/

void ForAll::gen_code(std::fstream &m_file) {
    if (this->parent) {
        m_file << "#pragma omp for" << std::endl;
    }
    m_file << "for(int " << this->gen_bound.identifier << " = " << this->gen_bound.lower << "; "
           << this->gen_bound.identifier;
    m_file << "<= " << this->gen_bound.upper << "; " << this->gen_bound.identifier << "++) {" << std::endl;
    if (this->expression.length() > 0)
        m_file << this->expression << ";" << std::endl;
    int tmp = this->child.index();
    if (tmp == 0) {
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
