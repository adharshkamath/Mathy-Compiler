#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

double clock()
{
    struct timeval Tp;
    int stat;
    stat = gettimeofday(&Tp, NULL);
    if (stat != 0)
        printf("Error return from gettimeofday: %d", stat);
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

void kernel()
{
    static float a[100 + 2][100 + 2] = { 0 }, final_u[100 + 2][100 + 2] = { 0 }, lu_l[100 + 2][100 + 2] =
        { 0 }, u[100 - 1 + 2][100 + 2] = { 0 }, l[100 + 2][100 - 1 + 2] = { 0 }, final_l[100 + 2][100 + 2] =
        { 0 }, lu_u[100 + 2][100 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                for (int k = 0; k <= i - 1; k++) {
#pragma omp atomic
                    lu_u[i][j] += l[i][k] * u[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                for (int k = 0; k <= j - 1; k++) {
#pragma omp atomic
                    lu_l[i][j] += l[i][k] * u[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
#pragma omp atomic
                final_u[i][j] = a[i][j] - lu_u[i][j];
            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
#pragma omp atomic
                final_l[i][j] = (a[i][j] - lu_u[i][j]) / final_u[j][j];
            }
        }
    }
}

int main()
{
    double start = 0.0, end = 0.0;
    start = clock();
    kernel();
    end = clock();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
