
%{ 
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #define YYSTYPE char*
    extern int yylex(void);
    int yyerror(const char *msg);
    bool success = true; 
    int errors = 0;
    int yycolumn = 0;
    int n=1;

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

identifier_list :   identifier_list ',' identifier  { strcat(strcat($$, ","), $3); insert($3, "float", "0"); }
                |   identifier                      { strcpy($$, $1); insert($1, "float", "0"); }
                |   initialize_id
                ;

initialize_id   :   identifier '=' number ;

identifier  :   IDENTIFIER dimensions
            ;

dimensions  :   '[' offset ']' dimensions   { strcat() }
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
            |   identifier '=' expression
            |   forall_stmt precisions        { if(strlen($2)>0) printf("Precision List FORALL : %s\n",$2); }
            |   prod_sum_stmt precisions     { if(strlen($2)>0) printf("Precision List PROD/SUM : %s\n",$2); }
            ;

precisions  :   '!' precision_list      { strcpy($$,$2); }
            |   %empty      { $$ = ""; printf("Default precision\n"); }
            ;

precision_list  :   precision_list ',' precision    { strcat(strcat($$, ","), $3); }
                |   precision
                |   %empty
                ;

precision   :   IDENTIFIER ':' PRECISION    { strcat(strcat($$, ","), $3); }
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
  
int main() {
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
