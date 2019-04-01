#include <stdio.h>
int i;
void print();
 
int main(int argc, char const *argv[])
{
	printf("1. i = %d\n", i);
	print();
	printf("2. i = %d\n", i);
	return 0;
}