#include <stdio.h>
#include <string.h>

void compare_2string (char *s1, char *s2) {
	if (s1 > s2)
		printf(">\n");
	else if (s1 < s2)
		printf("<\n");
	else
		printf("=\n");
}
int main(int argc, char const *argv[])
{
	char *s1 = "bcd", *s2 = "abcdcba";
	char *start, *end, *temp;

	printf("Test: %p\t%p\n", s1, s2 );
	//compare_2string (s1, s2);
	//while ()
	start = strchr (s2, 'b');
	temp = strchr (s2, 'c');
	end = strrchr (s2, 'b');
	printf("Test2: %p\t%p\t%p\t%p\n", s2, start, temp, end );

	return 0;
}