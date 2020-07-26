#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

extern int n;


int yywrite() {
    FILE *fptr;
    fptr = fopen("output.c", "w");
    char *basic =  "#include<stdio.h>\n#include<stdlib.h>\n#include<omp.h>\n\nint main()\n{\n}";
    fprintf(fptr, "%s", basic);
    fclose(fptr);
}

void printSymbolTable() {
    for(int i=1; i<n; i++) {
        printf("%s - %s\n", table[i].token, table[i].type);
    }
}

void insert(char* token, char* type, char* value) {
    int index = hash(token);
    if(!isPresent(token)){
        strcpy(table[index].token, token);
        strcpy(table[index].type, type);
        if(strcmp(type, "float") == 0)
            table[index].value.fval = strtof(value, NULL);
        if(strcmp(type, "long_double") == 0)
            table[index].value.ldval = strtold(value, NULL);
        if(strcmp(type, "double") == 0)
            table[index].value.dval = strtod(value, NULL);
        printf("%s - %s\n", token, type);
    }
    return;
}

int isPresent(char *token) {
    if(strcmp(token, table[hash(token)].token) == 0) return 1;
    return 0;
} 


int setBounds(char* token, int op_id, int lower, int upper) {

}

int hash(char* lexeme) {
    size_t i = 0;
    int length = strlen(lexeme), hash = 0;
    while (i != length) {
        hash += lexeme[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return (hash % HASH_TABLE_SIZE + HASH_TABLE_SIZE) % HASH_TABLE_SIZE;
}


int newIdentifier(char* token, char* type, char* value) {
    int index = hash(token);
    if(!isPresent(token)){
        strcpy(table[index].token, token);
        strcpy(table[index].type, type);
        if(strcmp(type, "float") == 0)
            table[index].value.fval = strtof(value, NULL);
        if(strcmp(type, "long_double") == 0)
            table[index].value.ldval = strtold(value, NULL);
        if(strcmp(type, "double") == 0)
            table[index].value.dval = strtod(value, NULL);
        printf("%s - %s\n", token, type);
    }
    return 0;
}

