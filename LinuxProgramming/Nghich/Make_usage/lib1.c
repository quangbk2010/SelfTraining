#include <lib1.h>

void print_simple_1 () {
	char *s = "Hello, I am ";
	printf("%s", s);
}

void print_stdt (student_t stdt) {
	printf("id: %d\tname: %s\n", stdt.id, stdt.name);
}

student_t register_stdt (int id, char *name) {
	student_t stdt;
	stdt.id = id;
	strcpy (stdt.name, name);
	return stdt;
}