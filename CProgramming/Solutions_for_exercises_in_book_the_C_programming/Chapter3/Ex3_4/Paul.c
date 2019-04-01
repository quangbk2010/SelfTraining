/*In a two's complement number representation, our version of itoa does not handle the largest negative number, that is, the value of n equal to -(2 to the power (wordsize - 1)) . 
Explain why not. Modify it to print that value correctly regardless of the machine on which it runs. 

Exercise 3-4 explanation: There are a number of ways of representing signed integers in binary, for example, signed-magnitude, excess-M, one's complement and two's complement. 
We shall restrict our discussion to the latter two. In a one's complement number representation, the binary represenation of a negative number is simply the binary representation 
of its positive counterpart, with the sign of all the bits switched. For instance, with 8 bit variables: 
                SIGNED  BINARY  UNSIGNED
                
                  25   00011001     25
                 -25   11100110    230

                 127   01111111    127
                -127   10000000    128

The implications of this are (amongst others) that there are two ways of representing zero (all zero bits, and all one bits), that the maximum range for a signed 8-bit number 
is -127 to 127, and that negative numbers are biased by (2^n - 1) (i.e. -I is represented by (2^n - 1) - (+I). In our example, so: 
                Bias = 2^8 - 1 = 255 = 11111111
                Subtract 25          = 00011001
                Equals               = 11100110

) In a two's complement representation, negative numbers are biased by 2^n, e.g.: 
         Bias = 2^8  = 100000000
         Subtract 25 =  00011001
         Equals      =  11100111

In other words, to find the two's complement representation of a negative number, find the one's complement of it, and add one. The important thing to notice is that the range 
of an 8 bit variable using a two's complement representation is -128 to 127, as opposed to -127 to 127 using one's complement. Thus, the absolute value of the largest negative 
number cannot be represented (i.e. we cannot represent +128). Since the itoa() function in Chapter 3 handles negative numbers by reversing the sign of the number before processing, 
then adding a '-' to the string, passing the largest negative number will result it in being translated to itself: 
                 -128            : 11111111
                 One's complement: 00000000
                 Subtract 1      : 11111111

Therefore, because (n /= 10) will be negative, the do-while loop will run once only, and will place in the string a '-', followed by a single character, (INT_MIN % 10 + '0'). 
We can remedy these two bugs in the following way: 1 - change 'while ((n /= 10) > 0)' to 'while (n /= 10)'. Since any fractional part is truncated with integer division, n will 
eventually equal zero after successive divides by 10, and 'n /= 10' will evaluate to false sooner or later. 2 - change 'n % 10 + '0'' to 'abs(n % 10) + '0'', to get the correct 
character. EX3_4.C shows the revised function, which will run correctly regardless of the number representation. 
*/

 /*

  EX3_4.C
  =======
  
  Suggested solution to Exercise 3-4
  
*/
    
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

void itoa_v1(int n, char s[]);
void itoa_v2(int n, char s[]);
void reverse(char s[]);

int main(void) {
    char buffer[20];
    
    printf("INT_MIN: %d\n", INT_MIN);
    itoa_v1(INT_MIN, buffer);
    printf("Buffer v1 : %s\n", buffer);
	
	itoa_v2(INT_MIN, buffer);
    printf("Buffer v2: %s\n", buffer);
    
    return 0;
}

/* itoa: convert n to characters in s */
void itoa_v1(int n, char s[])
{
	int i, sign;
	
	if ((sign = n) < 0) /* record sign */
		n = -n; /* make n positive */
	i = 0;
	do { /* generate digits in reverse order */
		s[i++] = n % 10 + '0'; /* get next digit */
	} while ((n /= 10) > 0); /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

void itoa_v2(int n, char s[]) {
    int i, sign;
    sign = n;
    
    i = 0;
    do {
        s[i++] = abs(n % 10) + '0';
    } while ( n /= 10 );
    if (sign < 0)
        s[i++] = '-';
    
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
