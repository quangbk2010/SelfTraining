#include <stdio.h>

void convert_to_bit (int n) {
	int i;
	printf("n = %d\n", n);
	printf("Binary type of n: ");
	for (i = 31; i >=0; i --) {
		if (n & (1 << i))
			printf("1");
		else 
			printf("0");
		if (i % 8 == 0)
			printf(" ");
	}
	printf("\n");
}
int main(int argc, char const *argv[])
{
	convert_to_bit (2);
	convert_to_bit (16);
	convert_to_bit (255);
	return 0;
}