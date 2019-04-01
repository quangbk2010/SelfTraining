#include <stdio.h>
#include <string.h>
#define LENGTH 3

typedef struct employee_s
{
	char name[30];
	int age;
	char sex[10];
	char major[20];
	int salary;

} employee_t;
employee_t entering (int i) {
	employee_t emp;
	char name[30];
	int age;
	char sex[10];
	char major[30];
	int salary;

	printf("Enter employee: %d\n", i);

	fflush(stdin);
	printf("Name: ");
	gets (name);

	printf("Age: ");
	scanf ("%d", &age);
	getchar();

	printf("Sex: ");
	scanf ("%s", sex);
	getchar();
	
	fflush(stdin);
	printf("Major: ");
	gets (major);

	printf("Salary: ");
	scanf ("%d", &salary);
	getchar();

	strcpy (emp.name, name);
	emp.age = age;
	strcpy (emp.sex, sex);
	strcpy (emp.major, major);
	emp.salary = salary;
	return emp;
}
void printing (employee_t emp) {
	printf("%s\t", emp.name);
	printf("%3d\t", emp.age);
	printf("%s\t", emp.sex);
	printf("%s\t", emp.major);
	printf("%5d\t", emp.salary);
}
int main(int argc, char const *argv[])
{
	employee_t list[LENGTH];
	int i;
	for (i = 0; i < LENGTH; i ++) {
		list[i] = entering (i+1);
	}
	printf("Name\t\tAge\tSex\tMajor\t\tSalary\n");
	for (i = 0; i < LENGTH; i ++) {
		printing (list[i]);
		printf("\n");
	}
	
	return 0;
}
