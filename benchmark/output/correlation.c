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
    static float corr[2600 - 1 + 2][2600 - 1 + 2] = { 0 }, std_dev[2600 - 1 + 2] =
        { 0 }, data[3000 - 1 + 2][2600 - 1 + 2] = { 0 }, cov[2600 - 1 + 2][2600 - 1 + 2] = { 0 }, mean[2600 - 1 + 2] =
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
            for (int j = 0; j <= 2600 - 1; j++) {
                for (int k = 0; k <= 3000 - 1; k++) {
#pragma omp atomic
                    cov[i][j] += ((data[k][i] - mean[i]) * (data[k][j] - mean[j])) / 99;
                }

            }
        }
#pragma omp for
        for (int t = 0; t <= 2600 - 1; t++) {
            for (int k = 0; k <= 3000 - 1; k++) {
#pragma omp atomic
                std_dev[t] += ((data[k][t] - mean[t]) * (data[k][t] - mean[t])) / 99;
            }

        }
#pragma omp for
        for (int p = 0; p <= 2600 - 1; p++) {
#pragma omp atomic write
            std_dev[p] = sqrt(std_dev[p]);
        }
#pragma omp for
        for (int i = 0; i <= 2600 - 1; i++) {
            for (int j = 0; j <= 2600 - 1; j++) {
#pragma omp atomic write
                corr[i][j] = cov[i][j] / (std_dev[i] * std_dev[j]);
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
