#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    float abcd[100 + 1][250 + 1] = { 0 }, c[150 + 1][300 + 1] = { 0 }, cd[150 + 1][250 + 1] =
        { 0 }, b[200 + 1][150 + 1] = { 0 }, d[300 + 1][250 + 1] = { 0 }, a[100 + 1][200 + 1] =
        { 0 }, ab[100 + 1][150 + 1] = { 0 };
    double start = 0.0, end = 0.0;
    start = omp_get_wtime();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 150; j++) {
                for (int k = 0; k <= 200; k++) {
                    ab[i][j] += a[i][k] * b[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 150; i++) {
            for (int j = 0; j <= 250; j++) {
                for (int k = 0; k <= 300; k++) {
                    cd[i][j] += c[i][k] * d[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 250; j++) {
                for (int k = 0; k <= 150; k++) {
                    abcd[i][j] += ab[i][k] * cd[k][j];
                }

            }
        }
    }
    end = omp_get_wtime();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
