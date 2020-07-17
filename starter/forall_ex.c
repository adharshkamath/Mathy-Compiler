/*
Possible output for an equation

    forall(i,j) where 0<=i,j<100 {
        c[i][j] = a[i][j] + b[i][j]
    }

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main () {
    int i, j;
    float a[100][100], b[100][100], c[100][100];
    for (i=0; i < 100; i++) {
        for(j=0; j < 100; j++) {
            a[i][j] = b[i][j] = c[i][j] = 0;
        }
    }
    #pragma omp parallel default(shared) private(i,j)
    {
        #pragma omp for schedule(dynamic, 10) collapse(2)
        for (i=0; i < 100; i++) {
            for(j=0; j < 100; j++) {
                c[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}