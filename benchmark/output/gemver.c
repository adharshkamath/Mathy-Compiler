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
    static float w[150 + 2][150 + 2] = { 0 }, z[150 + 2] = { 0 }, x[150 + 2] = { 0 }, v2[150 + 2] = { 0 }, u2[150 + 2] =
        { 0 }, v1[150 + 2] = { 0 }, u1[150 + 2] = { 0 }, y[150 + 2] = { 0 }, A[150 + 2][150 + 2] = { 0 }, beta =
        { 0 }, alpha = { 0 };
#pragma omp parallel
    {

#pragma omp atomic
        alpha = 2;
#pragma omp atomic
        beta = 3;
#pragma omp for
        for (int i = 0; i <= 150; i++) {
            for (int j = 0; j <= 150; j++) {
#pragma omp atomic
                A[i][j] += u1[i] * v1[j] + u2[i] * v2[j];
            }

        }
#pragma omp for
        for (int i = 0; i <= 150; i++) {
            for (int j = 0; j <= 150; j++) {
#pragma omp atomic
                x[i] += beta * A[j][i] * y[j];
            }

        }
#pragma omp for
        for (int i = 0; i <= 150; i++) {
#pragma omp atomic
            x[i] = x[i] + z[i];
        }
#pragma omp for
        for (int i = 0; i <= 150; i++) {
            for (int j = 0; j <= 150; j++) {
#pragma omp atomic
                w[i][j] += alpha * A[i][j] * x[i];
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
