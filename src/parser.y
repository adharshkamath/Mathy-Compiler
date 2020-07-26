
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
    char type[15] = "float";
    int c = 1;
    char *curid;
    void *curr_stmt;
      
%} 


%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT UNARY_OPERATOR OPERATOR COMPARISON FORALL SIGMA PRODUCT WHERE LEFT_SQUARE	RIGHT_SQUARE LEFT_PAREN	RIGHT_PAREN	RIGHT_CURLY LEFT_CURLY

%union {
    struct tableEntry id;
    union node gen_expr;
    struct bound loop_bound;
    char* string;
};

%type <string> INT_CONSTANT FLOAT_CONSTANT UNARY_OPERATOR OPERATOR COMPARISON WHERE IDENTIFIER
%type <string> LEFT_SQUARE	RIGHT_SQUARE LEFT_PAREN	RIGHT_PAREN	RIGHT_CURLY LEFT_CURLY dimensions offset offset_type control number
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

identifier  :   IDENTIFIER { curid = $1; newIdentifier(curid, type, "0"); } dimensions   {  }
            ;

dimensions  :   dimensions LEFT_SQUARE offset RIGHT_SQUARE { strcat(curid, $2); strcat(curid, $3); strcat(curid, $4); }
            |   %empty
            ;
            
offset  :   offset_type { strcpy($$, $1); }
        |   offset OPERATOR offset_type { strcpy($$, strcat(strcat($1, $2), $3)); }
        ;

offset_type :   INT_CONSTANT    { strcpy($$, $1); }
            |   IDENTIFIER      { strcpy($$, $1); }
            ;

number      :   INT_CONSTANT
            |   FLOAT_CONSTANT
            ;

expression  :   term    { $$ = $1; }
            |   term OPERATOR expression    { strcat($1, $2); strcat($1, $3); $$=$1; }
            |   identifier '=' expression       {  }
            |   forall_stmt
            |   prod_sum_stmt
            ;

term    :   identifier          { strcpy($$, $1); }
        |   number      { strcpy($$, $1); }
        ;

forall_stmt :   FORALL LEFT_PAREN IDENTIFIER RIGHT_PAREN WHERE bound {  } LEFT_CURLY statements RIGHT_CURLY { printf("Forall %s %s %s %s\n", $3, $5, $6, $8); };

prod_sum_stmt  :   control LEFT_PAREN expression RIGHT_PAREN WHERE bound { printf("Control expr %s\n", $3); };

control :   PRODUCT { $$ = "prod"; }
        |   SIGMA { $$ = "sigma"; }
        ;

bound   :   INT_CONSTANT COMPARISON IDENTIFIER COMPARISON INT_CONSTANT { $$.lowerBound = atoi($1); 
                                                                            $$.upperBound = atoi($5); 
                                                                            $$.token = $3; };

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
