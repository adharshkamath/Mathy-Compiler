#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    float cholesky[100 + 1][100 + 1] = { 0 }, L[100 + 1][100 + 1] = { 0 }, temp_sum = { 0 };
#pragma omp parallel
    {

#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int k = 0; k <= i; k++) {
                temp_sum += L[i][k] * L[i][k];
            }

            cholesky[i][i] = sqrt(L[i][i] - temp_sum);
        }
#pragma omp for
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= i; j++) {
                for (int k = 0; k <= j; k++) {
                    temp_sum += L[i][k] * L[j][k];
                }

                cholesky[i][j] = (L[i][i] - temp_sum) / cholesky[j][j];
            }
        }
    }
    return 0;
}
