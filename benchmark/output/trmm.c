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
    static float A[150 - 1 + 1][150 - 1 + 1] = { 0 }, B[150 - 1 + 1][250 - 1 + 1] = { 0 }, alpha = { 0 };
#pragma omp parallel
    {

#pragma omp atomic
        alpha = 10;
#pragma omp for
        for (int i = 0; i <= 150 - 1; i++) {
            for (int j = 0; j <= 250 - 1; j++) {
                for (int k = i + 1; k <= 150 - 1; k++) {
#pragma omp atomic
                    B[i][j] += A[k][i] * B[k][j];
                }

#pragma omp atomic
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
