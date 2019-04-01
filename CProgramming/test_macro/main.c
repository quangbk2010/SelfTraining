#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <lib.h>

/* Transfer more argument into function */
#define TRACE_LOG(fmt, args...) fprintf(stdout, fmt, ##args);

/* continous line */
#define NUMBERS 1,\
				2,\
				3
int arr[3] = {NUMBERS};

/* # Operator in macro*/
#define PRINT_INT(x) printf(#x " = %d %s\n", x, #x);
#define PRINT_INT_2(x) printf(#x " = %d\n", x)     

/* ## operator */	
#define GENERIC_MAX(type, x, y)\
type type##_max(type x, type y) \
{\
	return (x > y ? x : y);\
}

GENERIC_MAX (float, x, y)

/* Replacement */
#define FOR_ALL_HELLO(i)\
	for(i = 1; i <= 3; i ++)\
	{ \
	
#define FOR_ALL_HELLO_END(i) }
#

/\
* 
macro in a function, affect as global variable 
*/
void print_n () {
	#define ANUMBER 1
	#define ACHAR "ARNOLD"
	printf("ANUMBER = ");
}

/\
*
*/ # /*
*/ defi\
ne FO\
O 10\
20

#define VARDEF char *pArr;
#define CONSTCHAR char *string = "\nArray definition\n";
void printfArr () {
    VARDEF
    CONSTCHAR
    pArr = (char *) malloc (10 * sizeof (char));
    if (pArr) {
       strcpy (pArr, string);
    }
    printf ("%s", pArr);
}
int main() {
	int i=1, j=2;
	i++;
	i+++j;
	
/\
* 
Test tracelog macro with multiple argument 
*/
	TRACE_LOG("Array: ");
	for (i = 0; i < 3; i ++) {
		TRACE_LOG ("arr[%d] = %d\t", i, arr[i]);
	}
	printf("\n");

	/* Test print_int macro  */
	PRINT_INT (i)
    int them = 5;
    PRINT_INT (them)
	PRINT_INT_2 (i);

	/* test generic_max */
	printf("max = %f\n", float_max (1, 2));

	/* test for_all_hello */
	FOR_ALL_HELLO(i)
		printf("%d\n", i);
		FOR_ALL_HELLO_END(i)

	/* test print_n */
	print_n ();
	printf("%d %s %d\n", ANUMBER, ACHAR, FOO);
\
	//my_print ();
    printfArr ();
	return 0;
}
