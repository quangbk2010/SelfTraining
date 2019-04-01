#include "lib2.h"
#define FILE_NAME "List.txt"
#define DATA_LEN  20

int strsplit (char *string, char part[10][20]);
int read_file (int id[10], char name[10][20]);
void print_list (char list[10][20], int length);

int main(int argc, char const *argv[])
{
	char stdt_list[10][20]; // class: 10 students, each student's name has maximum length is 20
	int id[10], length_class, i;
	length_class = read_file (id, stdt_list);
	print_list (stdt_list, length_class);
	
	return 0;
}

int strsplit (char string[25], char part[2][20]) {
	char *temp;
	int i = 0;
	const char *space = " ";
	temp = strtok (string, space);
	while (temp != NULL) {
		strcpy (part[i], temp);
		i ++;
		temp = strtok (NULL, space);
	}
	return i;
}

int read_file (int id[10], char name[10][20]) {
	FILE *fp;
	char *line, part[2][20]; // each line has 2 part, maximum of each part size is 20
	size_t line_len;
	int i = 0;
	fp = fopen ("List.txt", "rt");
	while (getline (&line, &line_len, fp) != -1) {
		strsplit (line, part);
		id[i] = atoi (part[0]);
		strcpy (name[i], part[1]);
		i ++;
	}
	fclose (fp);
	return i; // length of file
}

void print_list (char list[10][20], int length) {
	int i;
	printf("Class list:\nId Name\n");
	for (i = 0; i < length; i ++) {
		printf("%d %s\n", i + 1, list[i]);
	}

	printf ("\n-------Introduction:-----------\n");
	for (i = 0; i < length; i ++) {
		print_simple_2 (list[i]);
	}
}