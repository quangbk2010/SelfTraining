#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

double my_sqrt(double x) {
	assert(x == 0.0);
	double a = sqrt(1/x);
	return a;
}

int main(int argc, char const *argv[])
{
	printf("sqrt +2 = %g\n", my_sqrt(2.0));
	printf("sqrt -2 = %g\n", my_sqrt(-0.0));
	return 0;
}