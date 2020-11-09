#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    static float a[100 + 1] = { 0 }, sum = { 0 };
    double start = 0.0, end = 0.0;
    start = omp_get_wtime();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            sum += a[i];
        }

        sum = sum / 100;
    }
    end = omp_get_wtime();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
