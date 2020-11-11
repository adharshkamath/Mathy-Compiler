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
    static float B[2000 - 1 + 2][2600 - 1 + 2] = { 0 }, A[2000 - 1 + 2][2000 - 1 - 1 + 2] =
        { 0 }, C[2000 - 1 - 1 + 2][2600 - 1 + 2] = { 0 }, beta = { 0 }, temp2 = { 0 }, alpha = { 0 };
#pragma omp parallel
    {

#pragma omp atomic write
        alpha = 1.500000;
#pragma omp atomic write
        beta = 1.200000;
#pragma omp for
        for (int i = 0; i <= 2000 - 1; i++) {
            for (int j = 0; j <= 2600 - 1; j++) {
#pragma omp atomic write
                temp2 = 0;
                for (int k = 0; k <= i - 1; k++) {
#pragma omp atomic
                    C[k][j] += alpha * B[i][j] * A[i][k];
                }

                for (int k = 0; k <= i - 1; k++) {
#pragma omp atomic
                    temp2 += B[k][j] * A[i][k];
                }

#pragma omp atomic write
                C[i][j] = beta * C[i][j] + alpha * B[i][j] * A[i][i] + alpha * temp2;
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
