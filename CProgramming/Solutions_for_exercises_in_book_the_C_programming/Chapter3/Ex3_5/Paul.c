/*
 Write the function itob(n,s,b) that converts the integer n into a base b character representation in the string s . 
 In particular, itob(n,s,16) formats n as a hexadecimal integer in s . 
*/

 /*

  EX3_5.C
  =======
  
  Suggested solution to Exercise 3-5
  
*/
    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
    
void itob(int n, char s[], int b);
void itob_v2(int n, char s[], int b);
void reverse(char s[]);

int main(void) {
    char buffer[40];
    int i;
    
	printf ("itob function\n");
    for ( i = 2; i <= 20; ++i ) {
        itob(255, buffer, i);
        printf("Decimal 255 in base %-2d : %s\n", i, buffer);
    }
	
	printf ("itob_v2 function\n");
	 for ( i = 2; i <= 20; ++i ) {
        itob_v2(255, buffer, i);
        printf("Decimal 255 in base %-2d : %s\n", i, buffer);
    }
	for ( i = 2; i <= 20; ++i ) {
        itob_v2(INT_MIN, buffer, i);
        printf("Decimal INT_MIN in base %-2d : %s\n", i, buffer);
    }
    return 0;
}


/*  Stores a string representation of integer n
    in s[], using a numerical base of b. Will handle
    up to base-36 before we run out of digits to use.  */

void itob(int n, char s[], int b) {
    static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, sign;
    
    if ( b < 2 || b > 36 ) {
        fprintf(stderr, "EX3_5: Cannot support base %d\n", b);
        exit(EXIT_FAILURE);
    }
    
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = digits[n % b];
    } while ((n /= b) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void itob_v2(int n, char s[], int b) {
    static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, sign;
    
    if ( b < 2 || b > 36 ) {
        fprintf(stderr, "EX3_5: Cannot support base %d\n", b);
        exit(EXIT_FAILURE);
    }
    
    sign = n;
    i = 0;
    do {
        s[i++] = digits[abs (n % b)];
    } while (n /= b);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/*  Reverses string s[] in place  */

void reverse(char s[]) {
    int c, i, j;
    for ( i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}