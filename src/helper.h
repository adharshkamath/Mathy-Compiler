#include <stdbool.h>

#ifndef HELPER_H
#define HELPER_H

#define HASH_TABLE_SIZE 100
#define NUM_TABLES 10

#define FORALL_STMT 1
#define SIGMA_STMT 2
#define PRODUCT_STMT 3
#define EXPRN_STMT 4

union data {
    float fval;
    double dval;
    long double ldval;
};
struct tableEntry {
    char* token;
    char* type;
    bool isArray;
    union data value;
    struct arraySize* arrSize;
} table[HASH_TABLE_SIZE];

struct bound {
    char* token;
    int lowerBound, upperBound;
};

struct forAll {
    struct bound* forBound;
    bool parent;
    char* expr;
    struct node* next;
    struct node* nest;
};

struct sigma_prod {
    bool parent;
    char* rhs, *lhs;
    struct bound* sum_prod_bound;
    struct node* next;
};


struct node {
    union {
        struct forAll forall;
        struct sigma_prod sum_prod;
        char* exprn;
    };
    int type;
};

struct arraySize {
    int size;
    struct arraySize* next;
};

int yywrite();

void printSymbolTable();

void insert(char* token, char* type, char* value);

int isPresent(char *token);

int setBounds(char* token, int op_id, int lower, int upper);

int hash(char *lexeme);

int newIdentifier(char* token, char* type, char* value);

#endif