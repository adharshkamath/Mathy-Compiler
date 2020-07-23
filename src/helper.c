#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

extern int n;

int yywrite() {

    FILE *fptr;
    fptr = fopen("output.c", "w");
    char *basic =  "#include<stdio.h>\n#include<stdlib.h>\n#include<omp.h>\n\nint main()\n";
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
            table[n].fval = strtof(value, NULL);
        if(strcmp(type, "long_double") == 0)
            table[n].ldval = strtold(value, NULL);
        if(strcmp(type, "double") == 0)
            table[n].dval = strtod(value, NULL);
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