#include <stdio.h>
#include "lib1.h"

typedef struct student_list_s {
	student_t last_stdt;
	student_t *stdt_list;
} student_list_t;

void print_simple_2 ();
int len_stdt_list (student_list_t *stdt_list);
void print_stdt_list (student_list_t *stdt_list);
student_list_t add_stdt_list_withId (student_list_t *stdt_list, int id);
student_list_t add_stdt_list_withStruct (student_list_t *stdt_list, student_t stdt);
student_list_t rm_stdt_list_withId (student_list_t *stdt_list, int id);
student_list_t rm_stdt_list_withStruct (student_list_t *stdt_list, student_t stdt);