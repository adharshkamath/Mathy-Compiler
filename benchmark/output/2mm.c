#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    float b[100 + 1][50 + 1] = { 0 }, a[50 + 1][100 + 1] = { 0 }, c[50 + 1][50 + 1] = { 0 };
#pragma omp parallel
    {

#pragma omp parallel for
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 50; j++) {
                for (int k = 0; k <= 100; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }

            }
        }
    }
    return 0;
}
