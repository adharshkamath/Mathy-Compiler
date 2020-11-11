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
    static float hz[60 - 1 + 2][80 - 1 + 2] = { 0 }, _fict_[40 - 1 + 2] = { 0 }, ex[60 - 1 + 2][80 - 1 + 2] =
        { 0 }, ey[0 + 2][80 - 1 + 2] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int t = 0; t <= 40 - 1; t++) {
            for (int j = 1; j <= 80 - 1; j++) {
#pragma omp atomic write
                ey[0][j] = _fict_[t];
            }
            for (int i = 1; i <= 60 - 1; i++) {
                for (int j = 0; j <= 80 - 1; j++) {
#pragma omp atomic write
                    ey[i][j] = ey[i][j] - 0.500000 * (hz[i][j] - hz[i - 1][j]);
                }
            }
            for (int i = 0; i <= 60 - 1; i++) {
                for (int j = 1; j <= 80 - 1; j++) {
#pragma omp atomic write
                    ex[i][j] = ex[i][j] - 0.500000 * (hz[i][j] - hz[i][j - 1]);
                }
            }
            for (int i = 0; i <= 59 - 1; i++) {
                for (int j = 0; j <= 79 - 1; j++) {
#pragma omp atomic write
                    hz[i][j] = hz[i][j] - 0.700000 * (ex[i][j + 1] - ex[i][j] + ey[i + 1][j] - ey[i][j]);
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
