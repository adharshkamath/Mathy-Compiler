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
    static float A[2000 - 1 + 2][2000 - 1 + 2] = { 0 }, B[2000 - 1 + 2][2600 - 1 + 2] = { 0 }, alpha = { 0 };
#pragma omp parallel
    {

#pragma omp atomic write
        alpha = 1.500000;
#pragma omp for
        for (int i = 0; i <= 2000 - 1; i++) {
            for (int j = 0; j <= 2600 - 1; j++) {
                for (int k = i + 1; k <= 2000 - 1; k++) {
#pragma omp atomic
                    B[i][j] += A[k][i] * B[k][j];
                }

#pragma omp atomic write
                B[i][j] = alpha * B[i][j];
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
