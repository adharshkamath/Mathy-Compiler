%skeleton "lalr1.cc"
%require "3.7.2"
%defines
%define api.parser.class { Parser }
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { mathy }
%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <variant>
    #include <stdint.h>
    #include "bounds.h"
    #include "forall.h"
    #include "gen_node.h"
    #include "sigma_prod.h"
    #include "helper.h"
    #include "files.h"

    using namespace std;

    namespace mathy {
        class Scanner;
        class Compiler;
    }

}


%code top
{
    #include <iostream>
    #include "scanner.h"
    #include "parser.hpp"
    #include "compiler.h"
    #include "location.hh"
    
    using namespace mathy;

    static Parser::symbol_type yylex(Scanner &scanner, Compiler &driver) {
        return scanner.get_next_token();
    }
    std::string current_id;
    std::vector<std::string> unfinished_vars, bound_ids;
    bool success = true;
}

%lex-param { mathy::Scanner &scanner }
%lex-param { mathy::Compiler &driver }
%parse-param { mathy::Scanner &scanner }
%parse-param { mathy::Compiler &driver }
%locations
%define parse.trace
%define parse.error verbose
%define api.token.prefix {TOKEN_}

%token END 0
%token <int> INTCONST;
%token <double> FLOATCONST;
%token <char> NEWLINE;
%token <std::string> IDENTIFIER;
%token <std::string> FORALL;
%token <std::string> SIGMA;
%token <std::string> WHERE;
%token <std::string> SQRT;
%token <std::string> PRODUCT;
%token <std::string> COMPARISON;
%token <std::string> OPERATOR;
%token <std::string> EQUALS;
%token LEFTPAR;
%token RIGHTPAR;
%token LEFTCURLY;
%token RIGHTCURLY;
%token LEFTSQR;
%token RIGHTSQR;
%token SEMICOLON;
%token COMMA;

%type<std::string> control identifier offset_type offset dimensions final intermediate_expr
%type< std::variant<GeneralNode, ForAll, SigmaProd, long int> > expression
%type< std::variant<GeneralNode*, ForAll*, SigmaProd*, long int> > program statement statements
%type<ForAll> forall_stmt
%type<SigmaProd> prod_sum_stmt
%type<Bound> bound
%type< std::variant<double, int> > number
%type< std::variant<std::string, double, int> > term

%%

program :   statements  {
                            int idxn = ($1).index();
                            int t_idx = root.index();
                            if(idxn == 0) {
                                mathy::gen_ptr = std::get<0>($1);
                                $$ = mathy::gen_ptr;
                                if(t_idx == 0) {
                                    mathy::gen_ptr = std::get<0>(root);
                                }
                                else if(t_idx == 1) {
                                    mathy::for_ptr = std::get<1>(root);
                                }
                                else if(t_idx == 2) {
                                    mathy::sp_ptr = std::get<2>(root);
                                }                                
                            }
                            else if(idxn == 1) {
                                mathy::for_ptr = std::get<1>($1);
                                $$ = mathy::for_ptr;
                                if(t_idx == 0) {
                                    mathy::gen_ptr = std::get<0>(root);
                                }
                                else if(t_idx == 1) {
                                    mathy::for_ptr = std::get<1>(root);
                                }
                                else if(t_idx == 2) {
                                    mathy::sp_ptr = std::get<2>(root);
                                }
                            }
                            else if(idxn == 2) {
                                mathy::sp_ptr = std::get<2>($1);
                                $$ = mathy::sp_ptr;
                                if(t_idx == 0) {
                                    mathy::gen_ptr = std::get<0>(root);
                                }
                                else if(t_idx == 1) {
                                    mathy::for_ptr = std::get<1>(root);
                                }
                                else if(t_idx == 2) {
                                    mathy::sp_ptr = std::get<2>(root);
                                }
                            }
                            initOutput();
                        }
        ;

statements  :   statements statement    {
                                            int idx = ($1).index();
                                            int t_idx = previous_stmt.index();
                                            if(idx == 0) {
                                                auto temp = std::get<0>($1);
                                                if(temp == 0) {
                                                    $1 = $2;
                                                }
                                                else {
                                                    int t1 = $2.index();
                                                    if(t1 == 0) {
                                                        auto t1v = std::get<0>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<0>(current_stmt);
                                                        if(t_idx == 0) {
                                                            std::cout << idx << " === " << std::endl;
                                                            auto t_type = std::get<0>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 1) {
                                                            auto t_type = std::get<1>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 2) {
                                                            auto t_type = std::get<2>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 3) {
                                                            previous_stmt = current_stmt;
                                                        }
                                                    }
                                                    else if(t1 == 1) {
                                                        auto t1v = std::get<1>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<1>(current_stmt);
                                                        if(t_idx == 0) {
                                                            auto t_type = std::get<0>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 1) {
                                                            auto t_type = std::get<1>(previous_stmt);
                                                            t_type->next = t2v;
                                                            auto hh = std::get<1>(t_type->next);
                                                            std::cout << "gen previous 2  " << " -- " << current_stmt.index() << std::endl;
                                                            auto nn = std::get<1>(current_stmt);
                                                            std::cout << t_type->expression << "| === |" << t_type->gen_bound.identifier <<  std::endl;
                                                        }
                                                        else if(t_idx == 2) {
                                                            auto t_type = std::get<2>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 3) {
                                                            previous_stmt = current_stmt;
                                                            std::cout << "gen previous " << " -- " << current_stmt.index() << std::endl;
                                                        }
                                                    }
                                                    else if(t1 == 2) {
                                                        auto t1v = std::get<2>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<2>(current_stmt);
                                                        if(t_idx == 0) {
                                                            auto t_type = std::get<0>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 1) {
                                                            auto t_type = std::get<1>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 2) {
                                                            auto t_type = std::get<2>(previous_stmt);
                                                            t_type->next = t2v;
                                                        }
                                                        else if(t_idx == 3) {
                                                            previous_stmt = current_stmt;
                                                            std::cout << "gen previous " << " -- " << current_stmt.index() << std::endl;
                                                        }
                                                    }
                                                }
                                                $$ = $1;
                                            }
                                            else if(idx == 1) {
                                                auto temp = std::get<1>($1);
                                                auto t_type = std::get<1>(previous_stmt);
                                                if(temp == 0) {
                                                    $1 = $2;
                                                }
                                                else {
                                                    int t1 = $2.index();
                                                    if(t1 == 0) {
                                                        auto t1v = std::get<0>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<0>(current_stmt);
                                                        t_type->next = t2v;
                                                    }
                                                    else if(t1 == 1) {
                                                        auto t1v = std::get<1>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<1>(current_stmt);
                                                        t_type->next = t2v;
                                                    }
                                                    else if(t1 == 2) {
                                                        auto t1v = std::get<2>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<2>(current_stmt);
                                                        t_type->next = t2v;
                                                    }
                                                }
                                                $$ = $1;
                                            }
                                            else if(idx == 2) {
                                               auto temp = std::get<2>($1);
                                                auto t_type = std::get<2>(previous_stmt);
                                                if(temp == 0) {
                                                    $1 = $2;
                                                }
                                                else {
                                                    int t1 = $2.index();
                                                    if(t1 == 0) {
                                                        auto t1v = std::get<0>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<0>(current_stmt);
                                                        t_type->next = t2v;
                                                    }
                                                    else if(t1 == 1) {
                                                        auto t1v = std::get<1>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<1>(current_stmt);
                                                        t_type->next = t2v;
                                                    }
                                                    else if(t1 == 2) {
                                                        auto t1v = std::get<2>($2);
                                                        temp->next = t1v;
                                                        auto t2v = std::get<2>(current_stmt);
                                                        t_type->next = t2v;
                                                    }
                                                }
                                                $$ = $1;
                                            }
                                            else if(idx == 3) {
                                                std::cout << "---- HERE stmts null ----- " << previous_stmt.index() << std::endl;
                                                if(previous_stmt.index() != 3 && (mathy::root).index() == 3) {
                                                    mathy::root = previous_stmt;
                                                }
                                                else if(current_stmt.index() != 3 && (mathy::root).index() == 3) {
                                                    mathy::root = current_stmt;
                                                }
                                                auto temp = std::get<3>($1);
                                                if(temp == 0) {
                                                    $1 = $2;
                                                    previous_stmt = current_stmt;
                                                    if(previous_stmt.index() == 1) {
                                                        auto ttm = std::get<1>(previous_stmt);
                                                        // std::cout << "FORALL Finally " << ttm->gen_bound.identifier << "  |" << ttm->expression << std::endl;
                                                    }
                                                }
                                                else {
                                                    // no code should reach here because if idx is 3, then statements is NULL so temp _will_ be 0
                                                }
                                                $$ = $1;
                                            }
                                            
                                        }
            |   %empty { $$ = NULL; }
            ;

final       :   NEWLINE { $$ = $1; }
            |   END {  }
            ;

statement   :   NEWLINE {
                            $$ = NULL;
                        }

            |   expression final    { 
                                        int rhs_index = ($1).index(); 
                                        if(rhs_index == 0) {
                                            auto gen_temp = std::get<0>($1);
                                            $$ = &gen_temp;
                                            auto gen_temp_t = std::get<0>(mathy::current_node);
                                            mathy::current_stmt = new GeneralNode(gen_temp_t);
                                            // std::cout << "GEN with "  << gen_temp_t.gen_bound.identifier << "  " <<  gen_temp_t.expression << " cur stmt " << current_stmt.index() << std::endl;
                                        } 
                                        else if(rhs_index == 1) {
                                            auto gen_temp = std::get<1>($1);
                                            $$ = &gen_temp;
                                            auto gen_temp_t = std::get<1>(mathy::current_node);
                                            mathy::current_stmt = new ForAll(gen_temp_t);
                                            // if(previous_stmt.index() == 0) {
                                            //     auto ttm = std::get<0>(previous_stmt);
                                            //     std::cout << "FORALL with " << gen_temp_t.gen_bound.identifier << "|  |" << ttm->expression << " cur stmt " << current_stmt.index() << std::endl;
                                            // }
                                            // if(previous_stmt.index() == 1) {
                                            //     auto ttm = std::get<1>(previous_stmt);
                                            //     std::cout << "FORALL with " << gen_temp_t.gen_bound.identifier << "|  |" << ttm->gen_bound.identifier << " cur stmt " << current_stmt.index() << std::endl;
                                            // }
                                            // if(previous_stmt.index() == 3) {
                                            //     auto ttm = std::get<3>(previous_stmt);
                                            //     std::cout << "FORALL with PROBLEMSSSSSS " << std::endl;
                                            // }
                                        } 
                                        else if(rhs_index == 2) {
                                            auto gen_temp = std::get<2>($1);
                                            $$ = &gen_temp;
                                            auto gen_temp_t = std::get<2>(mathy::current_node);
                                            mathy::current_stmt = new SigmaProd(gen_temp_t);
                                            // auto ttm1 = std::get<2>(current_stmt);
                                            // std::cout << "TEMPPPP  |" << ttm1->gen_bound.identifier << "|  |" << ttm1->LHS << "|  |" << ttm1->RHS << std::endl;
                                            // if(previous_stmt.index() == 0) {
                                            //     auto ttm = std::get<0>(previous_stmt);
                                            //     std::cout << "SP with " << gen_temp_t.gen_bound.identifier << "  --- |" << ttm << std::endl;
                                            // }
                                            // if(previous_stmt.index() == 1) {
                                            //     auto ttm = std::get<1>(previous_stmt);
                                            //     std::cout << "SP with " << gen_temp_t.gen_bound.identifier << "  " << ttm->gen_bound.identifier << std::endl;
                                            // }
                                            // if(previous_stmt.index() == 3) {
                                            //     auto ttm = std::get<3>(previous_stmt);
                                            //     std::cout << "SP with PROBLEMSSSSSS " << std::endl;
                                            // }
                                        }
                                        
                                    }
            ;

identifier  :   IDENTIFIER  {
                                current_id = $1;
                                if(isVariableFinalized(current_id) == 0 && 
                                    std::find(bound_ids.begin(), bound_ids.end(), $1) == bound_ids.end())   {
                                        unfinished_vars.push_back(current_id);
                                    }
                            } 
                dimensions  { $$ = $1 + $3; if(isVariableFinalized(current_id) == 0 && 
                                                std::find(bound_ids.begin(), bound_ids.end(), $1) == bound_ids.end())   {
                                                int check = newVariable($1);
                                                if(check == 0) addArrDimension(current_id, $3);
                                            }
                            }
            ;

dimensions  :   dimensions LEFTSQR offset RIGHTSQR { $$ = $1 + "[" + $3 + "]"; }
            |   %empty  {  }
            ;
            
offset  :   offset_type { $$ = $1; }
        |   offset OPERATOR offset_type { $$ = $1 + $2 + $3; }
        ;

offset_type :   INTCONST    { $$ = to_string($1); }
            |   IDENTIFIER   { $$ = $1; }
            ;

number      :   INTCONST    { $$ = $1; }
            |   FLOATCONST  { $$ = $1; }
            ;

intermediate_expr   :   OPERATOR expression { auto eval = std::get<0>($2); $$ = $1 + eval.expression;  }
                    |   %empty      {  }

expression  :   term    { 
                            if(($1).index() == 0) {
                                $$ = GeneralNode(EXPRN_NODE, std::get<0>($1));
                            }
                            else {
                                auto nval = std::get<1>($1);
                                $$ = GeneralNode(EXPRN_NODE, to_string(nval));
                            }
                        }
            |   term OPERATOR expression    {
                                                if($3.index() == 0) {
                                                    std::string value;
                                                    if(($1).index() == 0)                        
                                                        value = std::get<0>($1);
                                                    else {
                                                        auto nval = std::get<1>($1);
                                                        value = to_string(nval);
                                                    }
                                                    auto gen_str = std::get<0>($3);
                                                    $$ = GeneralNode(EXPRN_NODE, value + $2 + gen_str.expression);
                                                }
                                            }
            |   identifier EQUALS expression    {   
                                                    if($3.index() == 0) {
                                                        auto gen_str = std::get<0>($3);
                                                        if(gen_str.node_type == 0) {
                                                            $$ = GeneralNode(EXPRN_NODE, $1 + $2 + gen_str.expression);
                                                            mathy::current_node = GeneralNode(EXPRN_NODE, $1 + $2 + gen_str.expression);
                                                        }
                                                        else if(gen_str.node_type == 4) {
                                                            $$ = GeneralNode(SQRT_NODE, $1 + $2 + gen_str.expression);
                                                            mathy::current_node = GeneralNode(SQRT_NODE, $1 + $2 + gen_str.expression);
                                                        }
                                                    }
                                                    else if($3.index() == 2) {
                                                        auto prod_sum = std::get<2>($3);
                                                        $$ = SigmaProd(prod_sum.gen_bound, prod_sum.node_type, prod_sum.RHS, $1);
                                                        mathy::current_node = SigmaProd(prod_sum.gen_bound, prod_sum.node_type, prod_sum.RHS, $1);
                                                    }
                                                    for(auto& var : unfinished_vars) {
                                                        finalizeVariable(var);
                                                    }
                                                    for(int i=unfinished_vars.size()-1; i>=0; i--) {
                                                        if(isVariableFinalized(unfinished_vars[i])) {
                                                            unfinished_vars.erase(unfinished_vars.begin() + i);
                                                        }
                                                    }
                                                }
            |   SQRT LEFTPAR expression RIGHTPAR    {
                                                        auto tempidx = $3.index();
                                                        if(tempidx == 0) {
                                                            auto tempstr = std::get<0>($3);
                                                            $$ = GeneralNode(SQRT_NODE, $1 + "(" + tempstr.expression + ")");
                                                        }
                                                        else {
                                                            std::cout << "erraneous syntax" << std::endl;
                                                        }                                    
                                                    }
            |   LEFTPAR expression RIGHTPAR  intermediate_expr  {
                                                                    auto tempidx = $2.index();
                                                                    if(tempidx == 0) {
                                                                        auto tempstr = std::get<0>($2);
                                                                        $$ = GeneralNode(EXPRN_NODE, "(" + tempstr.expression + ")" + $4);
                                                                    }
                                                                    else {
                                                                        std::cout << "erraneous syntax" << std::endl;
                                                                    }
                                                                }
            |   forall_stmt {   
                                $$ = $1;
                                for(auto& var : unfinished_vars) {
                                    finalizeVariable(var);
                                }
                                for(int i=unfinished_vars.size()-1; i>=0; i--) {
                                    if(isVariableFinalized(unfinished_vars[i])) {
                                        unfinished_vars.erase(unfinished_vars.begin() + i);
                                    }
                                }
                                freeBound($1.gen_bound.identifier);
                            }
            |   prod_sum_stmt   {   
                                    $$ = $1;
                                    for(auto& var : unfinished_vars) {
                                        finalizeVariable(var);
                                    }
                                    for(int i=unfinished_vars.size()-1; i>=0; i--) {
                                        if(isVariableFinalized(unfinished_vars[i])) {
                                            unfinished_vars.erase(unfinished_vars.begin() + i);
                                        }
                                    }
                                    freeBound($1.gen_bound.identifier);
                                }
            ;

term    :   identifier   { $$ = $1; }
        |   number  {
                        if(($1).index() == 0)                        
                            $$ = to_string(std::get<0>($1));
                        else {
                            $$ = to_string(std::get<1>($1));
                        }
                    }
        ;

forall_stmt :   FORALL LEFTPAR IDENTIFIER RIGHTPAR WHERE bound LEFTCURLY NEWLINE statements RIGHTCURLY  {
                                                                                                            $$ = ForAll($6, $9, $3);
                                                                                                            mathy::current_node = ForAll($6, mathy::current_stmt, $3);
                                                                                                            int tl = (mathy::current_stmt).index();
                                                                                                            if(tl == 0) {
                                                                                                                // std::cout << "Expression node" << std::endl;
                                                                                                                auto tk = std::get<0>(mathy::current_stmt);
                                                                                                                tk->parent = false;
                                                                                                                // std::cout << tk->gen_bound.identifier << std::endl;
                                                                                                            }
                                                                                                            else if(tl == 1) {
                                                                                                                // std::cout << "For node" << std::endl;
                                                                                                                auto tk = std::get<1>(mathy::current_stmt);
                                                                                                                tk->parent = false;
                                                                                                                // std::cout << tk->gen_bound.identifier << std::endl;
                                                                                                            }
                                                                                                            else if(tl == 2) {
                                                                                                                // std::cout << "SP node" << std::endl;
                                                                                                                auto tk = std::get<2>(mathy::current_stmt);
                                                                                                                tk->parent = false;
                                                                                                                // std::cout << tk->gen_bound.identifier << std::endl;
                                                                                                            }
                                                                                                        }
            ;

prod_sum_stmt  :   control LEFTPAR expression RIGHTPAR WHERE bound {    
                                                                        if($1.compare("sigma") == 0) {
                                                                            $$ = SigmaProd($6, SIGMA_NODE, $3);
                                                                        }
                                                                        else {
                                                                            $$ = SigmaProd($6, PRODUCT_NODE, $3);
                                                                        }
                                                                    }
                ;

control :   PRODUCT { $$ = $1; } 
        |   SIGMA { $$ = $1; }
        ;

bound   :   expression COMPARISON IDENTIFIER COMPARISON expression  {   
                                                                        auto l = std::get<0>($1); auto h = std::get<0>($5);
                                                                        $$ = Bound(l.expression, $2, $3, $4, h.expression);
                                                                        newBound($3, l.expression, h.expression);
                                                                        bound_ids.push_back($3);
                                                                    }
        ;
    
%%


void mathy::Parser::error(const location &loc , const std::string &message) {	
    std::cout << "Error: " << message << std::endl << "Error location: " << driver.location() << std::endl;
    success = false;
}
