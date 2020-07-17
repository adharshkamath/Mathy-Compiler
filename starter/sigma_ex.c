/*
Possible output for an equation

    sum = sigma(a[i]*a[i]) where 0<=i<=100

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main () {
    int i;
    float sum, a[100 + 1];
    sum = 0.0;
    for (i=0; i <= 100; i++) {
        a[i] = i;
    }
    #pragma omp parallel default(shared) private(i)
    {
        #pragma omp for schedule(dynamic, 10)
        for (i=0; i <= 100; i++) {
            #pragma omp critical 
            {
                sum += a[i]*a[i];
            }
        }
    }
    printf("Sum = %f\n", sum);
}