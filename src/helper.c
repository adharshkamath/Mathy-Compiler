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
    if(!isPresent(token)){
        strcpy(table[n].token, token);
        strcpy(table[n].type, type);
        if(strcmp(type, "float") == 0)
            table[n].value.fval = strtof(value, NULL);
        if(strcmp(type, "long_double") == 0)
            table[n].value.ldval = strtold(value, NULL);
        if(strcmp(type, "double") == 0)
            table[n].value.dval = strtod(value, NULL);
        n++;
    }
    return;
}

int isPresent(char *token) {
    for(int i=1; i<n; i++) {
        if(!strcmp(table[i].token, token)) {
            return i;
        }
    }
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