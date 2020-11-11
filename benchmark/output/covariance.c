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
    static float data[3000 - 1 + 2][2600 - 1 + 2] = { 0 }, cov[2600 - 1 + 2][2600 - 1 + 2] = { 0 }, mean[2600 - 1 + 2] =
        { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int x = 0; x <= 2600 - 1; x++) {
            for (int k = 0; k <= 3000 - 1; k++) {
#pragma omp atomic
                mean[x] += data[k][x] / 3000;
            }

        }
#pragma omp for
        for (int i = 0; i <= 2600 - 1; i++) {
            for (int j = i; j <= 2600 - 1; j++) {
                for (int k = 0; k <= 3000 - 1; k++) {
#pragma omp atomic
                    cov[i][j] += ((data[k][i] - mean[i]) * (data[k][j] - mean[j])) / 2999;
                }

#pragma omp atomic write
                cov[j][i] = cov[i][j];
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
