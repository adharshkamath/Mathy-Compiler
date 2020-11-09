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
                            int root_index = root.index();
                            switch(root_index) {
                                case 0: {
                                    mathy::gen_ptr = std::get<0>(root);
                                    break;
                                }
                                case 1: {
                                    mathy::for_ptr = std::get<1>(root);
                                    break;
                                }
                                case 2: {
                                    mathy::sp_ptr = std::get<2>(root);
                                    break;
                                }
                                default: {
                                    std::cout << "NULL statements!" << std::endl;
                                    break;
                                }
                            }
                            initOutput();
                        }
        ;

statements  :   statements statement    {
                                            // If stmts is NULL it means this is the first statement/node, possibly in a list of nodes/statements
                                            // So assign current_stmt to current_root
                                            // current_root is used to pass statements as child of ForAll
                                            // Handling nesting: (when statements is NULL)
                                            // Use an int to keep track of nesting levels
                                            // If nesting == 0 and root is NULL assign current_stmt to root
                                            // If nesting > 0 then we inside a nesting node
                                            // Increment nesting variable when a for loop starts OR just before `statements`
                                            // Handling all types of nodes for root:
                                            // Gen Node, SP Node = nesting will always be 0 and statements will be NULL for the first statement
                                            // ForAll Node = nesting will be zero only for the outermost node and that will be assigned to root  
                                            if(mathy::temp_prev.size() != 0 && $2.index() == 1) {
                                                mathy::previous_stmt = mathy::temp_prev.top();
                                                mathy::temp_prev.pop();
                                            }
                                            if($1.index() == 3) {
                                                if(mathy::nest_lvl == 0 && root.index() == 3) {
                                                    mathy::root = mathy::current_stmt;
                                                }
                                                mathy::current_root.push(mathy::current_stmt);
                                                $$ = $2;
                                                mathy::previous_stmt = mathy::current_stmt;
                                            }
                                            else if($1.index() == 0) {
                                                auto general_stmts_yy = std::get<0>($1);
                                                auto general_stmts = std::get<0>(mathy::previous_stmt);
                                                general_stmts_yy->next = $2;
                                                general_stmts->next = mathy::current_stmt;
                                                $$ = $2;
                                                mathy::previous_stmt = mathy::current_stmt;
                                            }
                                            else if($1.index() == 1) {
                                                auto for_stmts_yy = std::get<1>($1);
                                                auto for_stmts = std::get<1>(mathy::previous_stmt);
                                                for_stmts_yy->next = $2;
                                                for_stmts->next = mathy::current_stmt;
                                                $$ = $2;
                                                mathy::previous_stmt = mathy::current_stmt;
                                            }
                                            else if($1.index() == 2) {
                                                auto sp_stmts_yy = std::get<2>($1);
                                                auto sp_stmts = std::get<2>(mathy::previous_stmt);
                                                sp_stmts_yy->next = $2;
                                                sp_stmts->next = mathy::current_stmt;
                                                $$ = $2;
                                                mathy::previous_stmt = mathy::current_stmt;
                                            }
                                            mathy::current_stmt = NULL;
                                        }
            |   %empty { $$ = NULL; }
            ;

final       :   NEWLINE { $$ = $1; }
            |   END {  }
            ;

statement   :   NEWLINE {
                            $$ = new GeneralNode(EXPRN_NODE, "\n");
                            mathy::current_stmt = new GeneralNode(EXPRN_NODE, "\n");
                        }
            |   expression final    { 
                                        int rhs_index = ($1).index(); 
                                        if(rhs_index == 0) {
                                            auto gen_temp = std::get<0>($1);
                                            $$ = &gen_temp;
                                            auto gen_temp_t = std::get<0>(mathy::current_node);
                                            mathy::current_stmt = new GeneralNode(gen_temp_t);
                                        }
                                        else if(rhs_index == 1) {
                                            auto forall_temp = std::get<1>($1);
                                            $$ = &forall_temp;
                                            auto forall_temp_t = std::get<1>(mathy::current_node);
                                            mathy::current_stmt = new ForAll(forall_temp_t);
                                        } 
                                        else if(rhs_index == 2) {
                                            auto sp_temp = std::get<2>($1);
                                            $$ = &sp_temp;
                                            auto sp_temp_t = std::get<2>(mathy::current_node);
                                            mathy::current_stmt = new SigmaProd(sp_temp_t);
                                        }
                                        else {
                                            std::cout << "NULL expression! " << std::endl;
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

intermediate_expr   :   OPERATOR expression { auto left_expression = std::get<0>($2); $$ = $1 + left_expression.expression;  }
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
                                                else {
                                                    std::cout << "Invalid combination of Expression and Term!" << std::endl;
                                                }
                                            }
            |   identifier EQUALS expression    {   
                                                    if($3.index() == 0) {
                                                        auto string_expression = std::get<0>($3);
                                                        if(string_expression.node_type == 0) {
                                                            $$ = GeneralNode(EXPRN_NODE, $1 + $2 + string_expression.expression);
                                                            mathy::current_node = GeneralNode(EXPRN_NODE, $1 + $2 + string_expression.expression);
                                                        }
                                                        else if(string_expression.node_type == 4) {
                                                            $$ = GeneralNode(SQRT_NODE, $1 + $2 + string_expression.expression);
                                                            mathy::current_node = GeneralNode(SQRT_NODE, $1 + $2 + string_expression.expression);
                                                        }
                                                    }
                                                    else if($3.index() == 2) {
                                                        auto sigma_pi_expression = std::get<2>($3);
                                                        $$ = SigmaProd(sigma_pi_expression.gen_bound, sigma_pi_expression.node_type, sigma_pi_expression.RHS, $1);
                                                        mathy::current_node = SigmaProd(sigma_pi_expression.gen_bound, sigma_pi_expression.node_type, sigma_pi_expression.RHS, $1);
                                                    }
                                                    else {
                                                        std::cout << "Invalid type of expression!" << std::endl;
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
                                                        if($3.index() == 0) {
                                                            auto tempstr = std::get<0>($3);
                                                            $$ = GeneralNode(SQRT_NODE, "sqrt(" + tempstr.expression + ")");
                                                        }
                                                        else {
                                                            std::cout << "Erraneous syntax" << std::endl;
                                                        }                                                                                          
                                                    }
            |   LEFTPAR expression RIGHTPAR  intermediate_expr  {
                                                                    if($2.index() == 0) {
                                                                        auto tempstr = std::get<0>($2);
                                                                        $$ = GeneralNode(EXPRN_NODE, "(" + tempstr.expression + ")" + $4);
                                                                    }
                                                                    else {
                                                                        std::cout << "Erraneous syntax" << std::endl;
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

forall_stmt :   FORALL LEFTPAR IDENTIFIER RIGHTPAR WHERE bound LEFTCURLY NEWLINE {  mathy::nest_lvl++; mathy::temp_prev.push(mathy::previous_stmt); } 
                                                                                    statements RIGHTCURLY  {
                                                                                                            $$ = ForAll($6, $10, $3);
                                                                                                            // Create new node with current_root as child. 
                                                                                                            // Assign its address to current_stmt
                                                                                                            // Traverse to all the nest nodes and set parent to false
                                                                                                            mathy::current_node = ForAll($6, mathy::current_root.top(), $3);
                                                                                                            auto current_ptr = mathy::current_root.top();
                                                                                                            mathy::current_root.pop();
                                                                                                            while(current_ptr.index() != 3) {
                                                                                                                switch(current_ptr.index()) {
                                                                                                                    case 0: {
                                                                                                                                auto general_ptr = std::get<0>(current_ptr);
                                                                                                                                general_ptr->parent = false;
                                                                                                                                current_ptr = general_ptr->next;
                                                                                                                                break;
                                                                                                                    }
                                                                                                                    case 1: {
                                                                                                                            auto forall_ptr = std::get<1>(current_ptr);
                                                                                                                            forall_ptr->parent = false;
                                                                                                                            current_ptr = forall_ptr->next;
                                                                                                                            break;
                                                                                                                    }
                                                                                                                    case 2: {
                                                                                                                            auto spn_ptr = std::get<2>(current_ptr);
                                                                                                                            spn_ptr->parent = false;
                                                                                                                            current_ptr = spn_ptr->next;
                                                                                                                            break;
                                                                                                                    }
                                                                                                                    default: {
                                                                                                                            std::cout << "Invalid index!" << std::endl;
                                                                                                                            break;
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                            mathy::nest_lvl--;
                                                                                                            auto forall_node = std::get<1>(current_node);
                                                                                                            if(mathy::nest_lvl != 0) {
                                                                                                                forall_node.parent = false;
                                                                                                            }
                                                                                                            mathy::current_stmt = &(forall_node);
                                                                                                        }
            ;

prod_sum_stmt  :   control LEFTPAR expression RIGHTPAR WHERE bound  {    
                                                                        if($1.compare("sigma") == 0) {
                                                                            $$ = SigmaProd($6, SIGMA_NODE, $3);
                                                                        }
                                                                        else {
                                                                            $$ = SigmaProd($6, PRODUCT_NODE, $3);
                                                                        }
                                                                    }
                ;

control :   PRODUCT { $$ = "prod"; } 
        |   SIGMA { $$ = "sigma"; }
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
