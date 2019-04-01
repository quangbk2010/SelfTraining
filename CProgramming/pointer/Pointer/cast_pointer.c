#include <stdio.h>

int main(int argc, char const *argv[])
{
	/*char c = '1';
	char *d = &c;
	int *e = (int *) d;

	printf("%p %p %d\n", d, e, *e);*/

	int c = 1;
	int *d = &c;
	char *e = (char *) d;

	printf("%p %p %d\n", d, e, *e);
	return 0;
}
