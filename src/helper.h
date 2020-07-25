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
    struct bound forBound;
    void* next;
    bool parent;
    char* expr;
};

struct sigma_prod {
    char* rhs, *lhs;
    struct bound sum_prod_bound;
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

#endif