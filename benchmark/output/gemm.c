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
    static float B[250 + 2][100 + 2] = { 0 }, A[50 + 2][250 + 2] = { 0 }, C[50 + 2][100 + 2] = { 0 }, beta =
        { 0 }, alpha = { 0 };
#pragma omp parallel
    {

#pragma omp atomic write
        alpha = 10;
#pragma omp atomic write
        beta = 2;
#pragma omp for
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 100; j++) {
#pragma omp atomic write
                C[i][j] = C[i][j] * beta;
            }
            for (int k = 0; k <= 250; k++) {
                for (int j = 0; j <= 100; j++) {
#pragma omp atomic write
                    C[i][j] = C[i][j] + alpha * A[i][k] * B[k][j];
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
