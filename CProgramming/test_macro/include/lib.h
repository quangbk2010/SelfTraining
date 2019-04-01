// Can use this command to compile main.c file using library lib.h:
// cc -iquoteinclude main.c -o main // only for header files requested with: #include "filename"; before all directories specified by '-I' and standard system library.
// or:
// cc -Iinclude main.c -o main // directories named by '-I' are searched before the standard system include directories. Can be used for both "" and <>

#include <stdio.h>

void my_print () {
	printf("This is my print\n");
}