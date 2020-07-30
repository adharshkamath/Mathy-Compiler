
%{ 
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <getopt.h>
    #include "helper.h"
    extern FILE *yyin;
    extern int yylex(void);
    extern int yylineno, yycolumn;
    int yyerror(const char *msg);
    bool success = true; 
    int errors = 0;
    int yycolumn = 0;
    int n=1;
    char* type = "float";
    int c = 1;
    char *curid;
    void *curr_stmt;
    char **files;
    int filenum=0, numfiles=0,t_opt;     
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
%type <gen_expr> expression forall_stmt prod_sum_stmt term statement statements

%%

program :   statements { printSymbolTable(); };

statements  :   statements statement    { $$ = $2; if($2.type == FORALL_STMT) { printf("Forall Bound - %s \n", $2.forall.forBound->token); } }
            |   %empty      {  }
            ;

statement   :   '\n'    {  }
            |   expression '\n' { $$ = $1; }
            ;

identifier  :   IDENTIFIER { curid = $<string>1; newIdentifier(curid, type, "0"); $<id>$.token = $<string>1; $<id>$.type = type; } dimensions   { }
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
            |   term OPERATOR expression    { strcat($$.expression.exp, $1.expression.exp); 
                                                strcat($$.expression.exp, $<string>2); 
                                                strcat($$.expression.exp, $3.expression.exp); 
                                            }
            |   identifier EQUALS expression       {    if($3.type == SIGMA_STMT || $3.type == PRODUCT_STMT) { 
                                                            $$.type = $3.type;
                                                            $$.sum_prod.sum_prod_bound = $3.sum_prod.sum_prod_bound;
                                                            $$.sum_prod.lhs = $1.token; $$.sum_prod.rhs = $3.sum_prod.rhs;
                                                        }
                                                        else {
                                                            $$.type = EXPRN_STMT; strcat($$.expression.exp, $1.token); 
                                                            strcat($$.expression.exp, $<string>2); 
                                                            strcat($$.expression.exp, $3.expression.exp);
                                                        }
                                                    }
            |   forall_stmt     { $$.type = $1.type; $$.forall.forBound = $1.forall.forBound; $$.forall.nest = $1.forall.nest; }
            |   prod_sum_stmt   { $$ = $1; }
            ;

term    :   identifier          { $$.type = EXPRN_STMT; strcpy($$.expression.exp, $1.token); }
        |   number      { $$.type = EXPRN_STMT; strcpy($$.expression.exp, $1); }
        ;

forall_stmt :   FORALL LEFT_PAREN IDENTIFIER RIGHT_PAREN WHERE bound LEFT_CURLY statements RIGHT_CURLY {    $$.type = FORALL_STMT; 
                                                                                                            $$.forall.forBound = &($6); 
                                                                                                            $$.forall.nest = &($<gen_expr>9);
                                                                                                    };

prod_sum_stmt  :   control LEFT_PAREN expression RIGHT_PAREN WHERE bound { $$.type = strcmp($1, "sigma") == 0 ? SIGMA_STMT : PRODUCT_STMT; 
                                                                            $$.sum_prod.rhs = $3.expression.exp; 
                                                                            $$.sum_prod.sum_prod_bound = &($6); 
                                                                          };

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

    numfiles = argc-optind;
    files = argv;
    t_opt = optind;
    yyin = fopen(argv[optind], "r");
    if(yyin == NULL) {
        fprintf(stderr, "File %s does not exist!\n", argv[optind]);
        fprintf(stderr, "Usage: %s [-t type] file\n", argv[0]);
        exit(4);
    }
    filenum++;
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

int yywrap() {
    if(filenum == numfiles) return -1;
    else {
        extern int yycolumn, yylineno;
        yycolumn = 1;
        yylineno = 1;
        yyin = fopen(files[t_opt+filenum], "r");
        if(yyin == NULL) {
            fprintf(stderr, "File %s does not exist!\n", files[optind+filenum]);
            fprintf(stderr, "Usage: %s [-t type] file\n", files[0]);
            exit(4);
        }
        filenum++;
        return 0;
    }
}

int yyerror(const char *msg) {
    extern char* yytext;
    printf("\nProblem occured at line number %d, column number %d, near '%s' in %s\nError: %s\n", yylineno, yycolumn, yytext, files[t_opt+filenum], msg);
    errors++;
    success = false;
    return 1;
}
