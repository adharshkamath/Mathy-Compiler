#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    float c[50 + 1][50 + 1] = { 0 }, c_b[50 + 1][50 + 1] = { 0 }, a[50 + 1][50 + 1] = { 0 }, a_b[50 + 1][50 + 1] =
        { 0 }, beta = { 0 }, c_out[50 + 1][50 + 1] = { 0 }, b[50 + 1][50 + 1] = { 0 }, alpha = { 0 };
#pragma omp parallel
    {

        alpha = 10;
        beta = 2;
#pragma omp for
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 50; j++) {
                for (int k = 0; k <= 50; k++) {
                    a_b[i][j] += alpha * (a[i][k] * b[k][j]);
                }

            }
        }
#pragma omp for
        for (int p = 0; p <= 50; p++) {
            for (int q = 0; q <= 50; q++) {
                c_b[p][q] = c[p][q] * beta;
            }
        }
#pragma omp for
        for (int m = 0; m <= 50; m++) {
            for (int n = 0; n <= 50; n++) {
                c_out[m][n] = a_b[m][n] + c_b[m][n];
            }
        }
    }
    return 0;
}
