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
    static float x[7000 - 1 + 1] = { 0 }, tmp[5000 - 1 + 1] = { 0 }, A[5000 - 1 + 1][7000 - 1 + 1] =
        { 0 }, y[7000 - 1 + 1] = { 0 };
    double start = 0.0, end = 0.0;
    start = clock();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 7000 - 1; i++) {
            y[i] = 0;
        }
#pragma omp for
        for (int i = 0; i <= 5000 - 1; i++) {
            tmp[i] = 0;
            for (int j = 0; j <= 7000 - 1; j++) {
                tmp[i] += A[i][j] * x[j];
            }

            for (int k = 0; k <= 7000 - 1; k++) {
                y[k] = y[k] + (A[i][k] * tmp[i]);
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
