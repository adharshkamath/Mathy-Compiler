
%{ 
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <getopt.h>
    #define YYSTYPE char*
    extern FILE *yyin;
    extern int yylex(void);
    int yyerror(const char *msg);
    bool success = true; 
    int errors = 0;
    int yycolumn = 0;
    int n=1;
    char type[15] = "float";

    struct symbolTable {
        char token[100];
        char type[10];
        bool isArray;
        float fval;
        double dval;
        long double ldval;
        struct arraySize* arrSize;
    } table[200];

    struct arraySize {
        int size;
        struct arraySize* next;
    };

    int isPresent(char *token) {
        for(int i=1; i<n; i++) {
            if(!strcmp(table[i].token, token)) {
                return i;
            }
        }
        return 0;
    }

    void insert(char* token, char* type, char* value) {
        if(!isPresent(token)){
            strcpy(table[n].token, token);
            strcpy(table[n].type, type);
            if(strcmp(type, "float") == 0)
                table[n].fval = strtof(value, NULL);
            if(strcmp(type, "long_double") == 0)
                table[n].ldval = strtold(value, NULL);
            if(strcmp(type, "double") == 0)
                table[n].dval = strtod(value, NULL);
            n++;
        }
        return;
    }
    
    int setType(char* token, char* type) {

    }

    int setBounds(char* token, int op_id, int lower, int upper) {

    }

    void printSymbolTable() {
        for(int i=1; i<n; i++) {
            printf("%s - %s\n", table[i].token, table[i].type);
        }
    }
      
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

identifier_list :   identifier_list ',' identifier
                |   identifier
                |   initialize_id
                ;

initialize_id   :   identifier '=' number ;

identifier  :   IDENTIFIER dimensions
            ;

dimensions  :   '[' offset ']' dimensions
            |   %empty
            ;
            
offset  :   offset_type 
        |   offset_type OPERATOR offset
        ;

offset_type :   INT_CONSTANT
            |   IDENTIFIER
            ;

number      :   INT_CONSTANT
            |   FLOAT_CONSTANT
            ;

expression  :   term
            |   identifier UNARY_OPERATOR
            |   UNARY_OPERATOR  identifier
            |   term OPERATOR expression
            |   identifier '=' expression       { strcpy($$, $1); insert($1, type, "0"); }
            |   forall_stmt
            |   prod_sum_stmt
            ;

term    :   identifier
        |   number
        ;

forall_stmt :   FORALL '(' identifier_list ')' WHERE bounds '{' statements '}' ;

prod_sum_stmt  :   control '(' expression ')' WHERE bounds ;

control :   PRODUCT 
        |   SIGMA
        ;

bounds  :   bounds ',' bound
        |   bound
        ;

bound   :   INT_CONSTANT COMPARISON identifier_list COMPARISON INT_CONSTANT ;

%% 
  
int main(int argc, char **argv) {
    int opt;
    while((opt = getopt(argc, argv, "t:")) != -1) {
        if(opt == 't') {
            if(strcmp(optarg, "float") == 0);
            else if(strcmp(optarg, "double") == 0) {
                strcpy(type, optarg);
            }
            else if(strcmp(optarg, "long double") == 0) {
                strcpy(type, optarg);
            }
            else {
                fprintf(stderr, "Invalid type selected");
            }
        }
        else {
            fprintf(stderr, "Usage: %s [-t type] file\n", argv[0]);
        }
    }

    if(optind == argc) {
        fprintf(stderr, "Enter the name of the file to be compiled!\n");
        fprintf(stderr, "Usage: %s [-t type] file\n", argv[0]);
        exit(1);
    }

    yyin = fopen(argv[optind], "r");
    if(yyin == NULL) {
        fprintf(stderr, "File %s does not exist!\n", argv[optind]);
        fprintf(stderr, "Usage: %s [-t type] file\n", argv[0]);
        exit(1);
    }

    extern int yylineno;
    yyparse();
    if(success) {
        printf("OK\n");
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
