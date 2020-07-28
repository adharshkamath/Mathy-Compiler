
%{ 
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <getopt.h>
    #include "helper.h"
    extern FILE *yyin;
    extern int yylex(void);
    int yyerror(const char *msg);
    bool success = true; 
    int errors = 0;
    int yycolumn = 0;
    int n=1;
    char* type = "float";
    int c = 1;
    char *curid;
    void *curr_stmt;
      
%} 


%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT OPERATOR COMPARISON FORALL SIGMA PRODUCT WHERE LEFT_SQUARE RIGHT_SQUARE LEFT_PAREN RIGHT_PAREN RIGHT_CURLY LEFT_CURLY EQUALS

%union {
    struct tableEntry id;
    struct node gen_expr;
    struct bound loop_bound;
    char* string;
};

%type <string>  dimensions offset offset_type control number
%type <id> identifier
%type <loop_bound> bound
%type <gen_expr> expression forall_stmt prod_sum_stmt term

%%

program :   statements { printSymbolTable(); };

statements  :   statements statement
            |   %empty
            ;

statement   :   '\n'
            |   expression '\n' {  }
            ;

identifier  :   IDENTIFIER { curid = $<string>1; newIdentifier(curid, type, "0"); $<id>$.token = $<string>1; $<id>$.type = type; } dimensions   {  }
            ;

dimensions  :   dimensions LEFT_SQUARE offset RIGHT_SQUARE { strcat(curid, $<string>2); strcat(curid, $3); strcat(curid, $<string>4); }
            |   %empty  {  }
            ;
            
offset  :   offset_type { strcpy($$, $1); }
        |   offset OPERATOR offset_type { strcpy($$, strcat(strcat($1, $<string>2), $3)); }
        ;

offset_type :   INT_CONSTANT    { strcpy($$, $<string>1); }
            |   IDENTIFIER      { strcpy($$, $<string>1); }
            ;

number      :   INT_CONSTANT    { strcpy($$, $<string>1); }
            |   FLOAT_CONSTANT  { strcpy($$, $<string>1); }
            ;

expression  :   term    { $$ = $1; $$.type = EXPRN_STMT; }
            |   term OPERATOR expression    { strcat($$.exprn, $1.exprn); strcat($$.exprn, $<string>2); strcat($$.exprn, $3.exprn); }
            |   identifier EQUALS expression       { $$.type = EXPRN_STMT; strcat($$.exprn, $1.token); strcat($$.exprn, $<string>2); strcat($$.exprn, $3.exprn); }
            |   forall_stmt     { $$ = $1; }
            |   prod_sum_stmt   { $$ = $1; }
            ;

term    :   identifier          { $$.type = EXPRN_STMT; strcpy($$.exprn, $1.token); }
        |   number      { $$.type = EXPRN_STMT; strcpy($$.exprn, $1); }
        ;

forall_stmt :   FORALL LEFT_PAREN IDENTIFIER RIGHT_PAREN WHERE bound { $<gen_expr>$.type = FORALL_STMT; $<gen_expr>$.forall.forBound = &($6); } LEFT_CURLY statements RIGHT_CURLY {  };

prod_sum_stmt  :   control LEFT_PAREN expression RIGHT_PAREN WHERE bound { $$.type = strcmp($1, "sigma") == 0 ? SIGMA_STMT : PRODUCT_STMT; };

control :   PRODUCT { $$ = "prod"; }
        |   SIGMA { $$ = "sigma"; }
        ;

bound   :   INT_CONSTANT COMPARISON IDENTIFIER COMPARISON INT_CONSTANT { $$.lowerBound = atoi($<string>1); 
                                                                            $$.upperBound = atoi($<string>5); 
                                                                            $$.token = $<string>3; };

%% 
  
int main(int argc, char **argv) {
    int opt;
    while((opt = getopt(argc, argv, "t:")) != -1) {
        if(opt == 't') {
            if(strcmp(optarg, "float") == 0);
            else if(strcmp(optarg, "double") == 0) {
                strcpy(type, optarg);
            }
            else if(strcmp(optarg, "long_double") == 0) {
                strcpy(type, optarg);
            }
            else {
                fprintf(stderr, "Invalid type '%s' selected\n", optarg);
                exit(1);
            }
        }
        else {
            fprintf(stderr, "Usage: %s [-t type] file\n", argv[0]);
            exit(2);
        }
    }

    if(optind == argc) {
        fprintf(stderr, "Enter the name of the file to be compiled!\n");
        fprintf(stderr, "Usage: %s [-t type] file\n", argv[0]);
        exit(3);
    }

    yyin = fopen(argv[optind], "r");
    if(yyin == NULL) {
        fprintf(stderr, "File %s does not exist!\n", argv[optind]);
        fprintf(stderr, "Usage: %s [-t type] file\n", argv[0]);
        exit(4);
    }

    extern int yylineno;
    yyparse();
    if(success) {
        printf("OK\n");
        yywrite();
    }
    else {
        printf("\nParsing failed due to %d error(s)\n", errors);
    }
    return 0;
}

int yyerror(const char *msg) {
    extern char* yytext;
    extern int yylineno;
    extern int yycolumn;
    printf("\nProblem occured at line number %d, column number %d, near '%s'\nError: %s\n", yylineno, yycolumn, yytext, msg);
    errors++;
    success = false;
    return 1;
}
