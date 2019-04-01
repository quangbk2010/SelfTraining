#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void runlength_code () {
	char *s, *s_temp;
	s = (char *) malloc (30 * sizeof (char));
	s_temp = (char *) malloc (30 * sizeof (char));
	strcpy (s, "aaaaaabbbbbyyyy444***");
	int len = strlen (s);
	char c = 45;
	int i, count = 0, j = 0, temp;
	for (i = 0; i < len; ++i)
	{
		if (s[i] != '\0') {
			if (s[i] == s[i + 1])
				count ++;
			else {
				count ++;
				s_temp[j++] = count + 48; temp = j - 1;
				s_temp[j++] = s[i];
				s_temp[j++] = '-';
				count = 0;
			}
		}
		else
			s_temp[j] = '\0';
	}

	printf("Initial string: %s\nRunlength code: %s\n", s, s_temp);
}
int main(int argc, char const *argv[])
{
	runlength_code ();
	return 0;
}