#include <stdio.h>

double min (double x, double y) {
	return (x < y ? x : y);
}
double max (double x, double y) {
	return (x > y ? x : y);
}
double add (double x, double y) {
	return (x + y);
}
typedef double (*f_type) (double, double);

int main(int argc, char const *argv[])
{
	int i;
	f_type a_f[3];
	a_f[0] = min;
	a_f[1] = max;
	a_f[2] = add;
	for (i = 0; i < 3; ++i)
	{
		printf ("%.2f ", a_f[i](1.2, 2.3));
	}
	printf("\n");
	return 0;
}