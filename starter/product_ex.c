/*Possible output for an equation

    prod = product(a[i]) where 0<=i<=20

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main () {
    int i;
    float a[20 + 1];
    double prod;
    prod = 1.0;
    for (i=0; i <= 20; i++) {
        a[i] = i+1;
    }
    #pragma omp parallel default(shared) private(i)
    {
        #pragma omp for schedule(dynamic, 10)
        for (i=0; i <= 20; i++) {
            #pragma omp critical 
            {
                prod *= a[i];
            }
        }
    }
    printf("Product = %f\n", prod);
}