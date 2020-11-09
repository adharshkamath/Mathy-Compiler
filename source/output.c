#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    float res[50 + 1][100 + 1] = { 0 }, x[100 + 1] = { 0 }, a_x[100 + 1] = { 0 }, a[200 + 1][100 + 1] =
        { 0 }, a_t[100 + 1][200 + 1] = { 0 };
        float aa[] = {1,2,3,4,5}; float sum = 0, arrr[] = {0,0,0,0,0};
#pragma omp parallel
    {

#pragma omp for lastprivate(arrr)
        for (int j = 0; j < 5; j++) {
            arrr[j] = aa[j];
        }
    }
    for(int i=0; i<5; i++)
    printf("Value of arr[%d] = %f\n",i,arrr[i]);
    return 0;
}
