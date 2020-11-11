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
    static float A[4000 - 1 + 2][4000 - 1 - 1 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 4000 - 1; i++) {
            for (int j = 0; j <= i - 1; j++) {
                for (int k = 0; k <= j - 1; k++) {
#pragma omp atomic write
                    A[i][j] = A[i][j] - A[i][k] * A[j][k];
                }
#pragma omp atomic write
                A[i][j] = A[i][j] / A[j][j];
            }
            for (int k = 0; k <= i - 1; k++) {
#pragma omp atomic write
                A[i][i] = A[i][i] - A[i][k] * A[i][k];
            }
#pragma omp atomic write
            A[i][i] = sqrt(A[i][i]);
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
