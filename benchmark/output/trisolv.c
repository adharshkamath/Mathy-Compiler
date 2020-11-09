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
    static float L[250 + 2][250 - 1 + 2] = { 0 }, b[250 + 2] = { 0 }, x[250 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 250; i++) {
#pragma omp atomic
            x[i] = b[i];
            for (int j = 0; j <= i - 1; j++) {
#pragma omp atomic
                x[i] = x[i] - L[i][j] * x[j];
            }
#pragma omp atomic
            x[i] = x[i] / L[i][i];
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
