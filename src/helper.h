#include <stdbool.h>

#ifndef HELPER_H
#define HELPER_H

#define HASH_TABLE_SIZE 100
#define NUM_TABLES 10


union data {
    float fval;
    double dval;
    long double ldval;
};
struct tableEntry {
    char token[20];
    char type[10];
    bool isArray;
    union data value;
    struct arraySize* arrSize;
} table[HASH_TABLE_SIZE];

struct bound {
    char* token;
    int lowerBound, upperBound;
};

struct forAll {
    int type;
    struct bound forBound;
    bool parent;
    char* expr;
    union node* next;
};

struct sigma_prod {
    int type;
    char* rhs, *lhs;
    struct bound sum_prod_bound;
    union node* next;
};

struct expr {
    int type;
    char* expression;
};

union node {
    struct forAll* forall;
    struct sigma_prod* sum_prod;
    struct expr* exprn;
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