#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    float a[100 + 1] = { 0 }, sum = { 0 };
#pragma omp parallel
    {

#pragma omp parallel for
        for (int i = 0; i <= 100; i++) {
            sum += a[i];
        }

        sum = sum / 100;
    }
    return 0;
}