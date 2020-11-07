#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    float b[100 + 1][50 + 1] = { 0 }, a[50 + 1][100 + 1] = { 0 }, c[50 + 1] = { 0 };
#pragma omp parallel
    {

#pragma omp parallel for
        for (int j = 0; j <= 50; j++) {
#pragma omp parallel for sppp
#pragma omp parallel for sppp j
