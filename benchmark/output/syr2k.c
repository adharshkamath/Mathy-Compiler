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
    static float B[150 + 2][250 + 2] = { 0 }, A[150 + 2][250 + 2] = { 0 }, C[150 + 2][150 - 1 + 2] = { 0 }, beta =
        { 0 }, alpha = { 0 };
#pragma omp parallel
    {

#pragma omp atomic
        alpha = 10;
#pragma omp atomic
        beta = 2;
#pragma omp for
        for (int i = 0; i <= 150; i++) {
            for (int j = 0; j <= i - 1; j++) {
#pragma omp atomic
                C[i][j] = C[i][j] * beta;
            }
            for (int k = 0; k <= 250; k++) {
                for (int j = 0; j <= i; j++) {
#pragma omp atomic
                    C[i][j] += (B[i][k] * A[j][k] * alpha) + (B[j][k] * A[i][k] * alpha);
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
