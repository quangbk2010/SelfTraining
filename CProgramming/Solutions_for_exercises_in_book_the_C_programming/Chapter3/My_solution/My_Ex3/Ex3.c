#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_lower_case (char c) {
	if ('a' <= c && c <= 'z') 
		return 1;
	else 
		return 0;
}

int is_upper_case (char c) {
	if ('A' <= c && c <= 'Z') 
		return 1;
	else 
		return 0;
}

int is_digit (char c) {
	if ('0' <= c && c <= '9') 
		return 1;
	else 
		return 0;
}
void expand (char *s1, char *s2) {
	int i = 0, j = 0, k, range, cond = 0, abs_range;
	char *temp;
	int direc = 0;

	
	while (s2[i]) {
		if (s2[i] == '-' && (i > 0) && ((is_upper_case(s2[i - 1]) && is_upper_case(s2[i + 1])) || (is_lower_case(s2[i - 1]) && is_lower_case(s2[i + 1])) || (is_digit(s2[i - 1]) && is_digit(s2[i + 1]))))
			cond = 1;
		if (cond) {
			range = s2[i + 1] - s2[i - 1];
			abs_range = abs (range);
			if (range) {
				direc = range / abs_range;
				for (k = 1; k < abs_range; k ++) {
					s1[j ++] = s2[i - 1] + direc * k;
				}
			}
			else
				s1[j ++] = s2[i + 2];

			//printf("y: %d\t%c_%d\t%c_%d\n", abs_range, s2[i - 1], s2[i - 1], s2[i + 1], s2[i + 1]);
			
			cond = 0;
		}
		else {
			s1[j++] = s2[i];
		}
		i ++;
		//j ++;
	}
}

int main(int argc, char const *argv[])
{
	char *s[] = { "a-z-", "z-a-", "-1-6-", "a-ee-a", "a-R-L", "1-9-1", "5-5", NULL };
	char result[100];
	int i;

	while (s[i]) {
		expand (result, s[i]);
		printf("[%d]\nInitial: %s\nConverted: %s\n", i + 1, s[i], result);
		memset (result, '\0', 100);
		i ++;
	}
	return 0;
}