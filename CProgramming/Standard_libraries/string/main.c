#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// http://www.cplusplus.com/reference/cstring/
// variable: size_t

// macro: NULL


// Function
// !!! mem_XXX functions can handle with NULL character, but str_XXX functions
/*
**** Copying:
- memcpy: void *memcpy(void *dest, const void *src, size_t n)
   ---> Copy block of memory (function )
  ---> Copies n characters from src to dest
- memmove: void *memmove(void *dest, const void *src, size_t n)
   ---> Move block of memory (function )
  ---> Another function to copy n characters from str2 to str1
  
  !!!Note: memcpy just loops, while memmove performs a test to determine which direction to loop in to avoid corrupting the data
  
- strcpy: char *strcpy(char *dest, const char *src)
   ---> Copy string (function )
- strncpy: char *strncpy(char *dest, const char *src, size_t n)
  ---> Copy characters from string with the maximum n characters from the src

  !!!Note: strcpy stops when it encounters a NULL, memcpy does not.

**** Concatenation:
- strcat: char *strcat(char *dest, const char *src)
  ---> Concatenate strings (function )
  ---> Appends the string pointed to, by src to the end of the string pointed to by dest
- strncat: char *strncat(char *dest, const char *src, size_t n)
  ---> Append characters from string (function )
  ---> Appends the string pointed to, by src to the end of the string pointed to, by maximum number n characters.

**** Comparison:
- memcmp: int memcmp(const void *str1, const void *str2, size_t n)
    ---> Compare two blocks of memory (function )
     Compares the first n bytes of str1 and str2
- strcmp: int strcmp(const char *str1, const char *str2)
    ---> Compare two strings (function )
- strcoll: int strcoll(const char *str1, const char *str2)
    ---> Compare two strings using locale (function )
- strncmp: int strncmp(const char *str1, const char *str2, size_t n)
    ---> Compare characters of two strings (function )
	Compares at most the first n bytes of str1 and str2.
- strxfrm: size_t strxfrm(char *dest, const char *src, size_t n)
    ---> Transform string using locale (function )

**** Searching: {
- memchr: void *memchr(const void *str, int c, size_t n)
  --->  Locate character in block of memory (function )
   Searches for the first occurrence of the character c (an unsigned char) in the first n bytes of the string pointed to, by the argument str.
- strchr: char *strchr(const char *str, int c)
  --->  Locate first occurrence of character in string (function )
  ---> Searches for the first occurrence of the character c (an unsigned char) in the string pointed to, by the argument str
- strcspn: size_t strspn(const char *str1, const char *str2)
  --->  Get span until character in string (function )
  Calculates the length of the initial segment of str1 which consists entirely of characters not in str2.
- strpbrk: char *strpbrk(const char *str1, const char *str2)
  --->  Locate characters in string (function )
  Finds the first character in the string str1 that matches any character specified in str2.
- strrchr: char *strrchr(const char *str, int c)
  --->  Locate last occurrence of character in string (function )
  Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the argument str.
- strspn: size_t strspn(const char *str1, const char *str2)
  --->  Get span of character set in string (function )
  Calculates the length of the initial segment of str1 which consists entirely of characters in str2.
- strstr: char *strstr(const char *haystack, const char *needle) 
  --->  Locate substring (function )
- strtok: char *strtok(char *str, const char *delim)
  --->  Split string into tokens (function )
  Breaks string str into a series of tokens separated by delim
}

*** Other: {
- memset: void *memset(void *str, int c, size_t n)
  --->  Fill block of memory (function )
  ---> Copies the character c (an unsigned char) to the first n characters of the string pointed to, by the argument str
- strerror
  --->  Get pointer to error message string (function )
- strlen: size_t strlen(const char *str)
  --->  Get string length (function )
  Computes the length of the string str up to but not including the terminating null character.
}

*/

void test_copy () {
	
	char c[3];
	char b[5];//6
	char a[5];//6 -> right
	
	int len1, len2, len3;
	len1 = strlen (a);
	len2 = strlen (b);
	len3 = strlen (c);
	
	printf ("%p %p %p\n%d %d %d\n", a, c, b, len1, len2, len3);
	/*strcpy (a, "01234");
	strcpy (b, "56789");
	printf ("%p %p %p\n%d %d\n", a, c, b, len1, len2);
	/*
	memcpy (a, "01234", 6); // get the same results
	memcpy (b, "56789", 6);
	
	
	printf ("1. %s %s\n", a, b);
	
	memcpy(a,b,5);           // valid
	printf ("2. %s %s\n", a, b);
	memmove(a,b,5);          // Also valid, but slower than memcpy.
	printf ("3. %s %s\n", a, b);
	
	memcpy(&a[0], &a[1],4);  // Not valid since it overlaps.
	printf ("4. %s %s\n", a, b);
	memmove(&a[0], &a[1],4); // valid. 
	printf ("5. %s %s\n", a, b);*/
}

void  test_copy_2 () {
  char str1[9]; 
  strcpy (str1, "aabbccdd");
  printf( "The string: %s\n", str1 );
   memcpy( (str1+2), str1, 6 );
   printf( "New string: %s\n", str1 );

   strcpy (str1, "aabbccdd");

   printf( "The string: %s\n", str1 );
   memmove( (str1+2), str1, 6 );
   printf( "New string: %s\n", str1 );

}
void test_search (char *s, char ch) {
    char *ret;

    // memchr
    printf ("3.\n");
    ret = memchr (s, ch, 11);
    int i;
    printf ("s: ");
    for (i = 0; i < 12; i ++) {
        printf ("%c ", s[i]);
    }
    printf ("\n");
    printf ("String is: %s\nSearch by memchr: String after %c is %s, with the first char is %c\n", s,  ch, ret, *ret);

    // strchr
    ret = strchr (s, ch);    
    printf ("\nSearch by strchr: String after %c is %s, with the first char is %c\n",  ch, ret, *ret);
}

void test_search_2 () {
  char *s1, *s2, *s3;
  s1 = (char *) malloc (20);
  s2 = (char *) malloc (20);
  s3 = (char *) malloc (5);

  printf("%p %p %p\n", s1, s2, s3);

  strcpy (s1, "123, 4.5/6:7");
  strcpy (s3, ",/:"); // s3 = ",/:"; -> cannot free a pointer points to a constant (PTC), when declare (initialize) a PTC, 
                                       // the compiler understand that you don't intend to modify a block of memory once allocated, 
                                       // freeing memory is modify it
  
  printf("%p %p %p\n", s1, s2, s3);
  s2 = strtok (s1, s3);
  while (s2 != NULL) {
    printf(" %s\t %.1f\n", s2, atof (s2));
    s2 = strtok (NULL, s3); // strtok() keeps some data in static variables inside the function itself so it can continue 
                            // a searching from the point it left it the previous call. subsequent calls must specify NULL 
                            // as the first argument, which tells the function to continue 
                            // tokenizing the string you passed in first
  }

  free (s1);
  free (s2);
  free (s3);
  printf("%p %p %p\n", s1, s2, s3);
}
int main(int argc, char const *argv[])
{
    char *s;
    char ch = 'a';
    char *ret;

    s = (char *) malloc (20 * sizeof(char));
    //printf ("1.\n");
    memcpy (s, "123\t456\0abc\n", 12);

    //printf ("2.\n");
    //test_search (s, ch);
	test_copy ();
	return 0;
}
