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
    static float cholesky[100 + 1][100 + 1] = { 0 }, L[100 + 1][100 + 1] = { 0 }, temp_sum = { 0 };
    double start = 0.0, end = 0.0;
    start = clock();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int k = 0; k <= i; k++) {
                temp_sum += L[i][k] * L[i][k];
            }

            cholesky[i][i] = sqrt(L[i][i] - temp_sum);
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= i; j++) {
                for (int k = 0; k <= j; k++) {
                    temp_sum += L[i][k] * L[j][k];
                }

                cholesky[i][j] = (L[i][i] - temp_sum) / cholesky[j][j];
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
