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
    static float abcd[100 + 1][250 + 1] = { 0 }, c[150 + 1][300 + 1] = { 0 }, cd[150 + 1][250 + 1] =
        { 0 }, b[200 + 1][150 + 1] = { 0 }, d[300 + 1][250 + 1] = { 0 }, a[100 + 1][200 + 1] =
        { 0 }, ab[100 + 1][150 + 1] = { 0 };
    double start = 0.0, end = 0.0;
    start = clock();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 150; j++) {
                for (int k = 0; k <= 200; k++) {
                    ab[i][j] += a[i][k] * b[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 150; i++) {
            for (int j = 0; j <= 250; j++) {
                for (int k = 0; k <= 300; k++) {
                    cd[i][j] += c[i][k] * d[k][j];
                }

            }
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 250; j++) {
                for (int k = 0; k <= 150; k++) {
                    abcd[i][j] += ab[i][k] * cd[k][j];
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