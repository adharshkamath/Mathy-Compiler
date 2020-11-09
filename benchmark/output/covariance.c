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
    static float data[100 + 1][100 + 1] = { 0 }, cov[100 + 1][100 + 1] = { 0 }, mean[100 + 1] = { 0 };
    double start = 0.0, end = 0.0;
    start = clock();
#pragma omp parallel
    {

#pragma omp for
        for (int x = 0; x <= 100; x++) {
            for (int k = 0; k <= 100; k++) {
                mean[x] += data[k][x] / 100;
            }

        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                for (int k = 0; k <= 100; k++) {
                    cov[i][j] += ((data[k][i] - mean[i]) * (data[k][j] - mean[j])) / 99;
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
