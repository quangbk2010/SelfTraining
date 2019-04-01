#include <stdio.h>
#include <string.h>

typedef struct student_s {
	int id;
	char *name;
} student_t;

void print_simple_1 ();
void print_stdt (student_t stdt);
student_t register_stdt (int id, char *name);

