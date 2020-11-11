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
    static float A[2799 - 1 + 2][2799 - 1 + 2] = { 0 }, B[2799 - 1 + 2][2799 - 1 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int t = 0; t <= 1000 - 1; t++) {
            for (int i = 1; i <= 2799 - 1; i++) {
                for (int j = 1; j <= 2799 - 1; j++) {
#pragma omp atomic write
                    B[i][j] = 0.200000 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
                }
            }
            for (int p = 1; p <= 2799 - 1; p++) {
                for (int q = 1; q <= 2799 - 1; q++) {
#pragma omp atomic write
                    A[p][q] = 0.200000 * (B[p][q] + B[p][q - 1] + B[p][1 + q] + B[1 + p][q] + B[p - 1][q]);
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
