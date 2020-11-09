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
    static float A[99 + 2] = { 0 }, B[99 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int t = 0; t <= 250; t++) {
            for (int i = 1; i <= 99; i++) {
#pragma omp atomic
                B[i] = 0.333330 * (A[i] + A[i - 1] + A[i + 1]);
            }
            for (int i = 1; i <= 99; i++) {
#pragma omp atomic
                A[i] = 0.333330 * (B[i] + B[i - 1] + B[i + 1]);
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
