#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    static float a[100 + 1][100 + 1] = { 0 }, final_u[100 + 1][100 + 1] = { 0 }, lu_l[100 + 1][100 + 1] =
        { 0 }, u[100 - 1 + 1][100 + 1] = { 0 }, l[100 + 1][100 - 1 + 1] = { 0 }, final_l[100 + 1][100 + 1] =
        { 0 }, lu_u[100 + 1][100 + 1] = { 0 };
    double start = 0.0, end = 0.0;
    start = omp_get_wtime();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                for (int k = 0; k <= i - 1; k++) {
                    lu_u[i][j] += l[i][k] * u[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                for (int k = 0; k <= j - 1; k++) {
                    lu_l[i][j] += l[i][k] * u[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                final_u[i][j] = a[i][j] - lu_u[i][j];
            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                final_l[i][j] = (a[i][j] - lu_u[i][j]) / final_u[j][j];
            }
        }
    }
    end = omp_get_wtime();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
