/*
 Write a version of itoa that accepts three arguments instead of two. 
 The third argument is a minimum field width; the converted number must be padded with blanks on the left if necessary to make it wide enough. 
*/

 /*

  EX3_6.C
  =======
  
  Suggested solution to Exercise 3-6
  
*/

#include <stdio.h>
#include <limits.h>
#include <string.h>

void itoa(int n, char s[], int width);
void itoa_v2(int n, char s[], int width); // the converted number will be padded with blanks on the right if necessary.
void reverse(char s[]);

int main(void) {
    char buffer[20];
	int test1 = -21 % 10;
	int test2 = test1 + '0';
	printf ("%d\t%d\t%d\n", test1, '0', test2);
  
    itoa(-123, buffer, 6);
    printf("Buffer v1:%s\tendl\n", buffer);
	
	itoa_v2(-123, buffer, 6);
    printf("Buffer v2:%s\tendl\n", buffer);
	
    itoa(INT_MIN, buffer, 20);
    printf("Buffer v1:%s\tendl\n", buffer);
	
	itoa_v2(INT_MIN, buffer, 20);
    printf("Buffer v2:%s\tendl\n", buffer);
    
    return 0;
}

void itoa(int n, char s[], int width) {
    int i, sign;
    
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
        printf("%d %% %d + '0' = %d\n", n, 10, s[i-1]);
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    
    while (i < width )    /*  Only addition to original function  */
        s[i++] = ' ';
    
    s[i] = '\0';
    reverse(s);
}

void itoa_v2(int n, char s[], int width) {
    int i, sign = n;
    
    i = 0;
    do {
        s[i++] = abs (n % 10) + '0';
        printf("%d %% %d + '0' = %d\n", -n, 10, s[i-1]);
    } while (n /= 10);
    if (sign < 0)
        s[i++] = '-';
	
    while (i < width )    /*  Only addition to original function  */
        s[i++] = ' ';
    
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[]) {
    int c, i, j;
    for ( i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


