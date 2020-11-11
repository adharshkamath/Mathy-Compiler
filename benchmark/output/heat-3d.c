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
    static float A[199 - 1 + 2][199 - 1 + 2][199 - 1 + 2] = { 0 }, B[199 - 1 + 2][199 - 1 + 2][199 - 1 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int t = 0; t <= 1000; t++) {
            for (int i = 1; i <= 199 - 1; i++) {
                for (int j = 1; j <= 199 - 1; j++) {
                    for (int k = 1; k <= 199 - 1; k++) {
#pragma omp atomic write
                        B[i][j][k] =
                            A[i][j][k] + 0.125000 * (A[i + 1][j][k] - 2.000000 * A[i][j][k] + A[i - 1][j][k]) +
                            0.125000 * (A[i][j + 1][k] - 2.000000 * A[i][j][k] + A[i][j - 1][k]) +
                            0.125000 * (A[i][j][k + 1] - 2.000000 * A[i][j][k] + A[i][j][k - 1]);
                    }
                }
            }
            for (int i = 1; i <= 199 - 1; i++) {
                for (int j = 1; j <= 199 - 1; j++) {
                    for (int k = 1; k <= 199 - 1; k++) {
#pragma omp atomic write
                        A[i][j][k] =
                            B[i][j][k] + 0.125000 * (B[i + 1][j][k] - 2.000000 * B[i][j][k] + B[i - 1][j][k]) +
                            0.125000 * (B[i][j + 1][k] - 2.000000 * B[i][j][k] + B[i][j - 1][k]) +
                            0.125000 * (B[i][j][k + 1] - 2.000000 * B[i][j][k] + B[i][j][k - 1]);
                    }
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
