#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    float a_t_r[200 + 1] = { 0 }, a_t[100 + 1][200 + 1] = { 0 }, p[100 + 1] = { 0 }, a[200 + 1][100 + 1] =
        { 0 }, r[200 + 1] = { 0 }, a_p[200 + 1] = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 200; i++) {
            for (int m = 0; m <= 100; m++) {
                a_p[i] += a[i][m] * p[m];
            }

        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 200; j++) {
                a_t[i][j] = a[j][i];
            }
        }
#pragma omp for
        for (int i = 0; i <= 200; i++) {
            for (int m = 0; m <= 200; m++) {
                a_t_r[i] += a_t[i][m] * r[m];
            }

        }
    }
    return 0;
}
