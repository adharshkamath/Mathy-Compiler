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
    static float x[250 - 1 + 2] = { 0 }, b[250 + 2] = { 0 }, y[250 - 1 + 2] = { 0 }, A[250 + 2][250 - 1 + 2] =
        { 0 }, w = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 250; i++) {
            for (int j = 0; j <= i - 1; j++) {
#pragma omp atomic
                w = A[i][j];
                for (int k = 0; k <= j - 1; k++) {
#pragma omp atomic
                    w = w - (A[i][k] * A[k][j]);
                }
#pragma omp atomic
                A[i][j] = w / A[j][j];
            }
            for (int j = i; j <= 250; j++) {
#pragma omp atomic
                w = A[i][j];
                for (int k = 0; k <= i - 1; k++) {
#pragma omp atomic
                    w = w - (A[i][k] * A[k][j]);
                }
#pragma omp atomic
                A[i][j] = w;
            }
        }
#pragma omp for
        for (int i = 0; i <= 250; i++) {
#pragma omp atomic
            w = b[i];
            for (int j = 0; j <= i - 1; j++) {
#pragma omp atomic
                w = w - A[i][j] * y[j];
            }
#pragma omp atomic
            y[i] = w;
        }
#pragma omp for
        for (int i = 0; i <= 250 - 1; i++) {
#pragma omp atomic
            w = y[250 - 1 - i];
            for (int j = 250 - i; j <= 250 - 1; j++) {
#pragma omp atomic
                w = w - A[249 - i][j] * x[j];
            }
#pragma omp atomic
            x[250 - 1 - i] = w / A[250 - 1 - i][250 - 1 - i];
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
