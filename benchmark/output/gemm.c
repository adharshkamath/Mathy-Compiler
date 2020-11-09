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
    static float c[50 + 1][50 + 1] = { 0 }, c_b[50 + 1][50 + 1] = { 0 }, a[50 + 1][100 + 1] =
        { 0 }, a_b[50 + 1][50 + 1] = { 0 }, beta = { 0 }, c_out[50 + 1][50 + 1] = { 0 }, b[100 + 1][50 + 1] =
        { 0 }, alpha = { 0 };
    double start = 0.0, end = 0.0;
    start = clock();
#pragma omp parallel
    {

        alpha = 10;
        beta = 2;
#pragma omp for
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 50; j++) {
                for (int k = 0; k <= 100; k++) {
                    a_b[i][j] += alpha * (a[i][k] * b[k][j]);
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 50; j++) {
                c_b[i][j] = c[i][j] * beta;
            }
        }
#pragma omp for
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 50; j++) {
                c_out[i][j] = a_b[i][j] + c_b[i][j];
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
