
%{ 
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <getopt.h>
    #include "helper.h"
    #define YYSTYPE char*
    extern FILE *yyin;
    extern int yylex(void);
    int yyerror(const char *msg);
    bool success = true; 
    int errors = 0;
    int yycolumn = 0;
    int n=1;
    char type[15] = "float";
    int c = 1;
      
%} 


%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT UNARY_OPERATOR OPERATOR COMPARISON FORALL SIGMA PRODUCT WHERE PRECISION

%%

program :   statements { printSymbolTable(); };

statements  :   statements statement
            |   %empty
            ;

statement   :   '\n'
            |   expression '\n'
            ;

identifier  :   IDENTIFIER dimensions
            ;

dimensions  :   '[' offset ']' dimensions
            |   %empty
            ;
            
offset  :   offset_type 
        |   offset_type OPERATOR offset
        ;

offset_type :   INT_CONSTANT    { $$ = $1; printf("Offset no. %d is a constant = %s\n",c++,$1); }
            |   IDENTIFIER      { $$ = $1; printf("Offset no. %d is = %s\n",c++,$1); }
            ;

number      :   INT_CONSTANT
            |   FLOAT_CONSTANT
            ;

expression  :   term
            |   term OPERATOR expression
            |   identifier '=' expression       { strcpy($$, $1); insert($1, type, "0"); }
            |   forall_stmt
            |   prod_sum_stmt
            ;

term    :   identifier          { strcpy($$, $1); insert($1, type, "0"); }
        |   number
        ;

forall_stmt :   FORALL '(' IDENTIFIER ')' WHERE bound '{' statements '}' ;

prod_sum_stmt  :   control '(' expression ')' WHERE bound ;

control :   PRODUCT 
        |   SIGMA
        ;

bound   :   INT_CONSTANT COMPARISON IDENTIFIER COMPARISON INT_CONSTANT {  };

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
