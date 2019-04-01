#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse (char s[]) {
	int i, j;
	int len = strlen (s);
	char temp;

	for (i = 0, j = len - 1; i < j; i ++, j --) {
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}
}
void itob (int n, char s[], int base) {
	int i = 0, sign;
	char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVXYZ";

	if (base < 2 || base > 36) {
		fprintf(stderr, "Cannot support this base\n");
		exit (1);
	}
	sign = n;
	if (n < 0)
		n = -n;
	do
	{
		s[i ++] =  digits [n % base];
	} while (n /= base);
	if (sign < 0)
		s[i ++] = '-';
	s[i] = '\0';
	reverse (s);
}

int main(int argc, char const *argv[])
{
	char buff[10];
	int i; 
	for (i = 2; i <= 20; ++i)
	{
		itob (255, buff, i);
		printf("Result with base %d: %s\n", i, buff);
	}
	return 0;
}