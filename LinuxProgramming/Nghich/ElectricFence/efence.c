#include <stdio.h>
#include "stdlib.h"

int main(int argc, char const *argv[])
{
	char *ptr = (char *) malloc (1024);
	ptr[0] = 0;

	ptr[1024] = 0;
	return 0;
}