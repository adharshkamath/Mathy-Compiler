#include <stdbool.h>

#ifndef HELPER_H
#define HELPER_H

struct symbolTable {
    char token[100];
    char type[10];
    bool isArray;
    float fval;
    double dval;
    long double ldval;
    struct arraySize* arrSize;
} table[200];

struct bound {
    char* token;
    int lowerBound, upperBound;
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

#endif