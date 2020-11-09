#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    static float res[50 + 1][100 + 1] = { 0 }, x[100 + 1] = { 0 }, a_x[100 + 1] = { 0 }, a[200 + 1][100 + 1] =
        { 0 }, a_t[100 + 1][200 + 1] = { 0 };
    double start = 0.0, end = 0.0;
    start = omp_get_wtime();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 200; j++) {
                a_t[i][j] = a[j][i];
            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int k = 0; k <= 100; k++) {
                a_x[i] += a[i][k] * x[k];
            }

        }
#pragma omp for
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 100; j++) {
                for (int k = 0; k <= 100; k++) {
                    res[i][j] += a_t[j][k] * a_x[k];
                }

            }
        }
    }
    end = omp_get_wtime();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
