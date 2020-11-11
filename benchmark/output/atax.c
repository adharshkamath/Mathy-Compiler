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
    static float x[2200 - 1 + 2] = { 0 }, y[2200 - 1 + 2] = { 0 }, A[1800 - 1 + 2][2200 - 1 + 2] =
        { 0 }, tmp[1800 - 1 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 1800 - 1; i++) {
            for (int j = 0; j <= 2200 - 1; j++) {
#pragma omp atomic
                tmp[i] += A[i][j] * x[j];
            }

            for (int k = 0; k <= 2200 - 1; k++) {
#pragma omp atomic
                y[k] += A[i][k] * tmp[i];
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
