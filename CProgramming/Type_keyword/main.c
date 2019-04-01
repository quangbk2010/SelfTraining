#include <stdio.h>

void count () {
	static int i = 0;
	i ++;
	printf("%d\n", i);
}

int main(int argc, char const *argv[])
{
	int i, m = 1, n = 2;
	//char s[]; // error
	for (i = 0; i < 10; ++i)
	{
		count ();
	}
	i = (m++, n += 2, m += n);
	printf("%d %d %d\n", i, m, n);

	return 0;
}