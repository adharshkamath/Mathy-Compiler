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
    static float p[1800 - 1 + 2] = { 0 }, q[2200 - 1 + 2] = { 0 }, r[2200 - 1 + 2] =
        { 0 }, A[2200 - 1 + 2][1800 - 1 + 2] = { 0 }, s[1800 - 1 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 2200 - 1; i++) {
            for (int j = 0; j <= 1800 - 1; j++) {
#pragma omp atomic
                s[j] += A[i][j] * r[i];
            }

            for (int j = 0; j <= 1800 - 1; j++) {
#pragma omp atomic
                q[i] += A[i][j] * p[j];
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
