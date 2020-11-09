#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    static float x[1900 - 1 + 1] = { 0 }, tmp[1900 - 1 + 1] = { 0 }, A[1900 - 1 + 1][1900 - 1 + 1] =
        { 0 }, y[2100 - 1 + 1] = { 0 };
    double start = 0.0, end = 0.0;
    start = omp_get_wtime();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 2100 - 1; i++) {
            y[i] = 0;
        }
#pragma omp for
        for (int i = 0; i <= 1900 - 1; i++) {
            tmp[i] = 0;
            for (int j = 0; j <= 1900 - 1; j++) {
                tmp[i] += A[i][j] * x[j];
            }

            for (int k = 0; k <= 2100 - 1; k++) {
                y[k] = y[k] + (A[i][k] * tmp[i]);
            }
        }
    }
    end = omp_get_wtime();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
