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
    static float a_t_r[200 + 2] = { 0 }, a_t[100 + 2][200 + 2] = { 0 }, p[100 + 2] = { 0 }, a[200 + 2][100 + 2] =
        { 0 }, r[200 + 2] = { 0 }, a_p[200 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 200; i++) {
            for (int m = 0; m <= 100; m++) {
#pragma omp atomic
                a_p[i] += a[i][m] * p[m];
            }

        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 200; j++) {
#pragma omp atomic
                a_t[i][j] = a[j][i];
            }
        }
#pragma omp for
        for (int i = 0; i <= 200; i++) {
            for (int m = 0; m <= 200; m++) {
#pragma omp atomic
                a_t_r[i] += a_t[i][m] * r[m];
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
