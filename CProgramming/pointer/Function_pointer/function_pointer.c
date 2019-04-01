#include <stdio.h>
#include <math.h>

/* Declare function give max number from 2 numbers */
double fmax (double x, double y) 
{
	return (x > y ? x : y);
}

/* Calculate absolute value of a number */
double cal_absolute (double x) {
	return (x > 0 ? x : -x);
}

/* Declare and can assign fmax function for function pointer */
double (*p_fmax) (double, double) ;

/* Function pointer is a argument of a function */
double cal_complicate (double (*f) (double), double x, double y) {
	return (f (x) + f (y)) / 2;
}

/* main function */
int main(int argc, char const *argv[])
{
	p_fmax = &fmax; /* can also use: p_fmax = fmax; */
	printf("\n max = %.*f\n", 2, (p_fmax) (1.2, 3.4)); /* can also use: p_fmax (1.2, 3.4) or (*p_fmax) (1.2, 3.4) */

	//printf("\n S = %.*f\n", 2, cal_complicate (cal_absolute, -1.2, 3.4));
	int i;
	for (i = 1; i < 3; ++i) {
		printf("%d\n", i);
	}

}

