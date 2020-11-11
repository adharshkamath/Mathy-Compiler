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
    static float c[1800 + 2][2400 + 2] = { 0 }, d[1600 + 2][2400 + 2] = { 0 }, a[1600 - 1 + 2][2200 + 2] =
        { 0 }, temp[1600 - 1 + 2][1800 + 2] = { 0 }, beta = { 0 }, b[2200 + 2][1800 + 2] = { 0 }, alpha = { 0 };
#pragma omp parallel
    {

#pragma omp atomic write
        alpha = 1.500000;
#pragma omp atomic write
        beta = 1.200000;
#pragma omp for
        for (int i = 0; i <= 1600 - 1; i++) {
            for (int j = 0; j <= 1800; j++) {
                for (int k = 0; k <= 2200; k++) {
#pragma omp atomic
                    temp[i][j] += alpha * a[i][k] * b[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 1600; i++) {
            for (int j = 0; j <= 2400; j++) {
#pragma omp atomic write
                d[i][j] = d[i][j] * beta;
                for (int k = 0; k <= 1800; k++) {
#pragma omp atomic
                    d[i][j] += temp[i][k] * c[k][j];
                }

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
