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

    using namespace std;

    namespace mathy {
        class Scanner;
        class Compiler;
    }

    std::string type;
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
%token <char> NEWLINE;
%token <std::string> IDENTIFIER;
%token <int> INTCONST;
%token <double> FLOATCONST;
%token LEFTPAR;
%token RIGHTPAR;
%token LEFTCURLY;
%token RIGHTCURLY;
%token LEFTSQR;
%token RIGHTSQR;
%token SEMICOLON;
%token <std::string> FORALL;
%token <std::string> SIGMA;
%token <std::string> WHERE;
%token <std::string> PRODUCT;
%token <std::string> COMPARISON;
%token <std::string> OPERATOR;
%token <std::string> EQUALS;
%token COMMA;

%type<std::string> control identifier offset_type offset dimensions
%type< std::variant<GeneralNode, ForAll, SigmaProd> > expression
%type< std::variant<GeneralNode*, ForAll*, SigmaProd*> > program statement statements
%type<ForAll> forall_stmt
%type<SigmaProd> prod_sum_stmt
%type<Bound> bound
%type< std::variant<double, int> > number
%type< std::variant<std::string, double, int> > term

%start program

%%

program :   statements { $$ = $1; };

statements  :   statements statement { $1 = $2; }
            |   %empty {  }
            ;

statement   :   NEWLINE  {  }
            |   expression NEWLINE { 
                                        int temp = ($1).index(); 
                                        if(temp == 0) {
                                            auto gen_temp = std::get<0>($1);
                                            $$ = &gen_temp;
                                        } 
                                        else if(temp == 1) {
                                            auto gen_temp = std::get<1>($1);
                                            $$ = &gen_temp;
                                        } 
                                        else if(temp == 2) {
                                            auto gen_temp = std::get<2>($1);
                                            $$ = &gen_temp;
                                        }
                                        
                                    }
            ;

identifier  :   IDENTIFIER  dimensions   { $$ = $1 + $2; }
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

expression  :   term { 
                        if(($1).index() == 0) {
                            $$ = GeneralNode(EXPRN_NODE, std::get<0>($1));
                        }
                        else {
                            auto nval = std::get<1>($1);
                            $$ = GeneralNode(EXPRN_NODE, to_string(nval));
                        }
                    }
            |   term OPERATOR expression { if($3.index() == 0) {
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
            |   identifier EQUALS expression { if($3.index() == 0) {
                                                    auto gen_str = std::get<0>($3);
                                                    $$ = GeneralNode(EXPRN_NODE, $1 + $2 + gen_str.expression);
                                                }
                                                else if($3.index() == 2) {
                                                    auto prod_sum = std::get<2>($3);
                                                    $$ = SigmaProd(prod_sum.gen_bound, prod_sum.node_type, prod_sum.RHS);
                                                    auto temp = std::get_if<SigmaProd>(&$$);
                                                    temp->set_lhs($1);
                                                }
                                            }
            |   forall_stmt { $$ = $1; }
            |   prod_sum_stmt { $$ = $1; }
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

forall_stmt :   FORALL LEFTPAR IDENTIFIER RIGHTPAR WHERE bound LEFTCURLY statements RIGHTCURLY  { 
                                                                                                    $$ = ForAll($6, $8, $3);
                                                                                                }
            ;

prod_sum_stmt  :   control LEFTPAR expression RIGHTPAR WHERE bound {    
                                                                        if($1.compare("sigma")) {
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

bound   :   term COMPARISON IDENTIFIER COMPARISON term  {
                                                            $$ = Bound($1, $2, $3, $4, $5);
                                                        }
        ;
    
%%


void mathy::Parser::error(const location &loc , const std::string &message) {	
    std::cout << "Error: " << message << std::endl << "Error location: " << driver.location() << std::endl;
}