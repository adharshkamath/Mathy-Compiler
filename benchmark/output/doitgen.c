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
    static float A[150 + 2][250 + 2][350 + 2] = { 0 }, C4[350 + 2][350 + 2] = { 0 }, sum[350 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 150; i++) {
            for (int j = 0; j <= 250; j++) {
                for (int k = 0; k <= 350; k++) {
                    for (int h = 0; h <= 350; h++) {
#pragma omp atomic
                        sum[k] += A[i][j][h] * C4[h][k];
                    }

                }
                for (int k = 0; k <= 350; k++) {
#pragma omp atomic
                    A[i][j][k] = sum[k];
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
