#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main() {
	float a[100+1]= {0}, sum= {0};
#pragma omp parallel
	{

