#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <variant>
#include <regex>
#include <algorithm>
#include "helper.h"

namespace mathy {
    std::unordered_map <std::string, std::pair<std::string, std::string>> bounds_table;
    std::unordered_map <std::string, std::vector<std::string>> variable_table;
    std::string data_type = "float";
    std::string output_name = "output";
    GeneralNode *gen_ptr = NULL;
    SigmaProd *sp_ptr = NULL;
    ForAll *for_ptr = NULL;
    std::variant<GeneralNode *, ForAll *, SigmaProd *, long int> current_stmt, root = NULL, previous_stmt = NULL;
    std::variant<GeneralNode, ForAll, SigmaProd, long int> current_node;

    int newVariable(const std::string &identifier) {
        if (variable_table.find(identifier) != variable_table.end()) {
            return -1;
        }
        if (bounds_table.find(identifier) != bounds_table.end()) {
            return -2;
        }
        variable_table[identifier] = std::vector<std::string>();
        return 0;
    }

    int newBound(const std::string &identifier, const std::string &low, const std::string &high) {
        int ret = 0;
        if (bounds_table.find(identifier) != bounds_table.end()) {
            ret = -1;
        }
        bounds_table[identifier].first = low;
        bounds_table[identifier].second = high;
        return ret;
    }

    std::string getBoundValue(const std::string &identifier) {
        if (isNumber(identifier)) return identifier;
        if (bounds_table.find(identifier) != bounds_table.end()) {
            if (isNumber(bounds_table[identifier].second))
                return bounds_table[identifier].second;
            else {
                std::string bound = bounds_table[identifier].second;
                std::string res = "";
                std::vector <std::string> ops;
                std::regex operands("[^+-/*]+");
                splitTerms(bound, operands, ops);
                int size = ops.size();
                int location = -1;
                for (int i = 0; i < size; i++) {
                    location += ops[i].length();
                    if (isNumber(ops[i])) {
                        res += ops[i];
                        if (location + 1 < bound.length()) {
                            res += bound[location + 1];
                            location++;
                        }
                    } else {
                        std::string temp = getBoundValue(ops[i]);
                        if (temp.length() == 0) {
                            res += ops[i];
                            if (location + 1 < bound.length()) {
                                res += bound[location + 1];
                                location++;
                            }
                        } else {
                            res += temp;
                            if (location + 1 < bound.length()) {
                                res += bound[location + 1];
                                location++;
                            }
                        }
                    }
                }
                return res;
            }

        }
        return "";
    }

    int addArrDimension(const std::string &identifier, std::string &bound) {
        if (!isVariableDeclared(identifier)) return -1;
        std::vector <std::string> ops;
        std::replace(bound.begin(), bound.end(), ']', '_');
        std::replace(bound.begin(), bound.end(), '[', '_');
        std::regex operands("[^_]+");
        splitTerms(bound, operands, ops);
        int size = ops.size();
        for (int i = 0; i < size; i++) {
            addArrDimension_util(identifier, ops[i]);
        }
        return 0;
    }

    int addArrDimension_util(const std::string &identifier, const std::string &bound) {
        std::string res = "";
        std::vector <std::string> ops;
        std::regex operands("[^+-/*]+");
        splitTerms(bound, operands, ops);
        int size = ops.size();
        int location = -1;
        for (int i = 0; i < size; i++) {
            location += ops[i].length();
            if (isNumber(ops[i]) && getBoundValue(ops[i]) == ops[i]) {
                res += ops[i];
                if (location + 1 < bound.length()) {
                    char op = bound[location + 1];
                    res += op;
                }
            } else {
                std::string temp = getBoundValue(ops[i]);
                if (temp.length() == 0) {
                    res += ops[i];
                    if (location + 1 < bound.length())
                        res += bound[location + 1];
                } else {
                    res += temp;
                    if (location + 1 < bound.length())
                        res += bound[location + 1];
                }
            }
        }
        res += "+1";
        variable_table[identifier].push_back(res);
        return 0;
    }

    bool isVariableFinalized(const std::string &identifier) {
        if (!isVariableDeclared(identifier)) {
            return false;
        } else {
            bool flag = true;
            for (auto &x : variable_table[identifier]) {
                auto temp = getBoundValue(x);
                if (!(temp.compare(x) == 0 && isNumber(x)))
                    flag = false;
            }
            return flag;
        }
    }

    int finalizeVariable(const std::string &identifier) {
        bool flag = true;
        for (auto &x : variable_table[identifier]) {
            auto temp = getBoundValue(x);
            if (!(temp.compare(x) == 0 && isNumber(x)))
                flag = false;
        }
        if (flag)
            return 1;
        else {
            for (int j = 0; j < variable_table[identifier].size(); j++) {
                std::string res = "";
                std::vector <std::string> ops;
                auto bound = variable_table[identifier][j];
                if (isNumber(bound) && bound.compare(getBoundValue(bound)) == 0) continue;
                std::regex operands("[^+-/*]+");
                splitTerms(bound, operands, ops);
                int size = ops.size();
                int location = -1;
                for (int i = 0; i < size; i++) {
                    location += ops[i].length();
                    if (isNumber(ops[i]) && getBoundValue(ops[i]).compare(ops[i])) {
                        res += ops[i];
                        if (location + 1 < bound.length()) {
                            res += bound[location + 1];
                            location++;
                        }
                    } else {
                        std::string temp = getBoundValue(ops[i]);
                        if (temp.length() == 0) {
                            res += ops[i];
                            if (location + 1 < bound.length()) {
                                res += bound[location + 1];
                                location++;
                            }
                        } else {
                            res += temp;
                            if (location + 1 < bound.length()) {
                                res += bound[location + 1];
                                location++;
                            }
                        }
                    }
                }
                variable_table[identifier][j] = res;
            }
        }
        return 0;
    }

    bool isVariableDeclared(const std::string &identifier) {
        return variable_table.find(identifier) != variable_table.end();
    }

    bool isBoundDeclared(const std::string &identifier) {
        return bounds_table.find(identifier) != bounds_table.end();
    }

    void printStuff() {
        std::cout << "------ Final stuff ------" << std::endl;
        for (auto x : variable_table) {
            std::cout << x.first << " - ";
            for (auto y : x.second) {
                std::cout << y << " ";
            }
            std::cout << std::endl;
        }
        for (auto x : bounds_table) {
            std::cout << x.first << " - " << x.second.first << ":" << x.second.second << std::endl;
        }
        std::cout << "------ Final stuff ------" << std::endl;
    }

    void initOutput() {
        std::fstream output;
        mathy::output_name += std::string(".c");
        output.open(output_name, std::ios::out);
        output << "#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>\n#include <omp.h>\n" \
                    "\nint main() {" << std::endl;
        declareVars(output);
        output << "#pragma omp parallel\n\t{\n" << std::endl;

        if (gen_ptr != NULL) {
            // gen_ptr->gen_code(output);
            traverse(gen_ptr);
        } else if (for_ptr != NULL) {
            // for_ptr->gen_code(output);
            traverse(for_ptr);
        } else if (sp_ptr != NULL) {
            // sp_ptr->gen_code(output);
            traverse(sp_ptr);
        } else {
            std::cout << "ERROR Program is NULL" << std::endl;
        }

        output << "}" << std::endl << "return 0;" << std::endl << "}" << std::endl;
    }

    void splitTerms(const std::string &str, const std::regex &reg, std::vector <std::string> &res) {
        auto words_begin = std::sregex_iterator(str.begin(), str.end(), reg);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            res.push_back((*i).str());
    }

    bool isNumber(const std::string &s) {
        std::regex rgx("([0-9]+[+/*-])*[0-9]+");
        return std::regex_match(s, rgx);
    }

    bool isString(const std::string &s) {
        return !isNumber(s);
    }

    void declareVars(std::fstream &output) {
        output << "\t" << data_type << " ";
        auto it = variable_table.begin();
        for (it; it != variable_table.end(); it++) {
            output << it->first;
            for (auto &y : it->second) {
                output << "[" << y << "]";
            }
            output << "= {0}, ";
        }
        long pos = output.tellp();
        output.seekp(pos - 2);
        output << ";" << std::endl;
    }

    void traverse(GeneralNode *genp) {
        std::cout << "GEN NODE" << std::endl;
        std::cout << genp->expression << std::endl;
        std::cout << genp->node_type << std::endl;
        int tt = (genp->next).index();
        if (tt == 0) {
            auto t = std::get<0>(genp->next);
            std::cout << "-----" << t << std::endl;
            if (t != 0)
                traverse(t);
        } else if (tt == 1) {
            auto t = std::get<1>(genp->next);
            if (t != 0)
                traverse(t);
        } else if (tt == 2) {
            auto t = std::get<2>(genp->next);
            if (t != 0)
                traverse(t);
        } else if (tt == 3) {
            std::cout << "Null next of gen" << std::endl;
            return;
        }
    }

    void traverse(ForAll *genp) {
        std::cout << "FOR NODE" << std::endl;
        int ttt = (genp->child).index();
        if (ttt == 0) {
            auto tr = std::get<0>(genp->child);
            traverse(tr);
        } else if (ttt == 1) {
            auto tr = std::get<1>(genp->child);
            traverse(tr);
        } else if (ttt == 2) {
            auto tr = std::get<2>(genp->child);
            traverse(tr);
        } else if (ttt == 3) {
            std::cout << "Null child of gen" << std::endl;
            return;
        }
        int tt = (genp->next).index();
        if (tt == 0) {
            auto t = std::get<0>(genp->next);
            traverse(t);
        } else if (tt == 1) {
            auto t = std::get<1>(genp->next);
            traverse(t);
        } else if (tt == 2) {
            auto t = std::get<2>(genp->next);
            traverse(t);
        } else if (tt == 3) {
            std::cout << "Null next of for" << std::endl;
            return;
        }
    }

    void traverse(SigmaProd *genp) {
        std::cout << "SP NODE" << std::endl;
        int tt = (genp->next).index();
        if (tt == 0) {
            auto t = std::get<0>(genp->next);
            traverse(t);
        } else if (tt == 1) {
            auto t = std::get<1>(genp->next);
            traverse(t);
        } else if (tt == 2) {
            auto t = std::get<2>(genp->next);
            traverse(t);
        } else if (tt == 3) {
            std::cout << "Null next of sp" << std::endl;
            return;
        }
    }

    void freeBound(const std::string &identifier) {
        if (!(isBoundDeclared(identifier))) {
            return;
        } else
            bounds_table[identifier].first = "";
        bounds_table[identifier].second = "";
        return;
    }

} // namespace mathy

using namespace mathy;
