#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double dTemp;
int iTemp;
char s[] = "3.14asdf";
char *ps;

/* 1. type: size_t, wchar_t, div_t, idiv_t
- size_t:
+ Unsigned integral type

+ Alias of one of the fundamental unsigned integer types.
  It is a type able to represent the size of any object in bytes: size_t is the type returned by the sizeof operator and is widely used 
  in the standard library to represent sizes and counts.

+ In <cstring>, it is used as the type of the parameter num in the functions memchr, memcmp, memcpy, memmove, memset, strncat, strncmp, 
strncpy and strxfrm, which in all cases it is used to specify the maximum number of bytes or characters the function has to affect.

It is also used as the return type for strcspn, strlen, strspn and strxfrm to return sizes and lengths.

- wchar_t:
+ The width of wchar_t is compiler-specific and can be as small as 8 bits. Consequently, programs that need to be portable across 
any C or C++ compiler should not use wchar_t for storing Unicode text. The wchar_t type is intended for storing compiler-defined 
wide characters, which may be Unicode characters in some compilers.
*/

/* 2. Macro: NULL, EXIT_FAILURE, EXIT_SUCCESS,  RAND_MAX, MB_CUR_MAX
*/


/* 3. Function: 
*/
// atof, atoi, strtod, strtol, strtoul: return number if the first character is digit or the first character is '-' and the second character is digit. 
// Unless, this will return 0.000
void test_atof () {
    printf("test atof function\n");
    dTemp = atof ("-1.2abc123");
    printf ("%.2f\n", dTemp);

    dTemp = atof (s);
    printf ("%.2f\n", dTemp);

    dTemp = atof (ps);
    printf ("%.2f\n", dTemp);
      
    strcpy (ps, s);// warning
    dTemp = atof (ps);
    printf ("%.2f\n", dTemp);
}

void test_atoi () {
    printf("test atoi function\n");
    iTemp = atoi ("-1.2abc123");
    printf ("%d\n", iTemp);

    iTemp = atoi (s);
    printf ("%d\n", iTemp);

    iTemp = atoi (ps);
    printf ("%d\n", iTemp);
      
    strcpy (ps, s);// warning
    iTemp = atoi (ps);
    printf ("%d\n", iTemp);
}

// strtod store the rest part of string, after cutting the number. 
void test_strtod () {
    printf("test strtod function\n");
    char *endPtr;

    dTemp = strtod ("-1.2abc123", &endPtr);
    printf ("%.2f, %s\n", dTemp, endPtr);

    dTemp = strtod ("abc123", &endPtr);
    printf ("%.2f, %s\n", dTemp, endPtr);

    dTemp = strtod (s, &endPtr);
    printf ("%.2f, %s\n", dTemp, endPtr);

    dTemp = strtod (ps, &endPtr);
    printf ("%.2f, %s\n", dTemp, endPtr);
      
    strcpy (ps, s);// warning
    dTemp = strtod (ps, &endPtr);
    printf ("%.2f, %s\n", dTemp, endPtr);
}

// calloc, malloc, realloc, free, 
void test_alloc () {
    int *i_p;
    i_p = (int *) malloc (10 * sizeof (int));
    free (i_p);
}

// abort, exit, atexit
/* 
 - abort: void abort(void)
 ----> Aborts the current process, producing an abnormal program termination. The function raises the SIGABRT signal (as if raise(SIGABRT) was 
 called). This, if uncaught, causes the program to terminate returning a platform-dependent unsuccessful termination error code to the 
 host environment.
 - exit: void exit(int status)
 ----> terminates the calling process immediately. Any open file descriptors belonging to the process are closed and any children of the process
 are inherited by process 1, init, and the process parent is sent a SIGCHLD signal.
 - atexit: int atexit(void (*func)(void))
 ----> causes the specified function func to be called when the program terminates. You can register your termination function anywhere you like,
 but it will be called at the time of the program termination.
*/

// getenv, system
/*
 - getenv: char *getenv (const char *name)
  ---> searches for the environment variable pointed to, by name and returns the associated value to the string.
 - system: int system (const char *command)
  ---> passes the command name or program name specified by command to the host environment to be executed by the command processor and 
  returns after the command has been completed

*/
// bsearch, qsort, abs, labs, div, ldiv, rand, srand, mblen
/*
 - bsearch: search if an item exists in an array
 - qsort: sort an array with quick sort algorithm.
  + void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));
     . base -- This is the pointer to the first element of the array to be sorted.
     . nitems -- This is the number of elements in the array pointed by base.
     . size -- This is the size in bytes of each element in the array.
     . compar -- This is the function that compares two elements.
 - abs: int abs(int x)
 - labs: long int labs(long int x);
 - div: div_t div(int numer, int denom); 
  ---> return a structure: {quot, rem}
 - rand: int rand(void)
    The C library function int rand(void) returns a pseudo-random number in the range of 0 to RAND_MAX. 
    RAND_MAX is a constant whose default value may vary between implementations but it is granted to be at least 32767
 - srand: void srand(unsigned int seed)
  ---> seeds the random number generator used by the function rand. 
       If we don't use this function before rand(), the rand() function is automatically seeded with a value of 1. 
       -> the result generated by rand() is the same.
 - mblen:
  Get length of multibyte character. Returns the size of the multibyte character pointed by pmb, examining at most max bytes.
*/
void test_rand () {
    int i;
    int arr [10];
    time_t t;
    srand ((unsigned) time (&t));
    //srand (time (NULL));
    for (i = 0; i < 10; ++i)
    {
        arr[i] = rand() % 10;
        printf("%d\t", arr[i]);
    }
}
void test_mblen () {
    char str[] = "test string";
    int length;
    wchar_t dest;

    length = mblen (NULL, 0);
    printf("%d\n", length);

    length = mblen (str, sizeof (str));
    printf("%d\n", length);
}

// mbstowcs, mbtowc, wcstombs, wctomb, 
/*
 - mbstowcs: size_t mbstowcs(schar_t *pwcs, const char *str, size_t n);
  ----> converts the string of multi-byte characters pointed to, by the argument str to the array pointed to by pwcs.
*/
/*
 - malloc, calloc, realloc, free
*/

int main () {
    ps = (char *) malloc (4 * sizeof (char));
    ps = s;   
    //test_strtod ();
    test_mbstowcs ();
}
