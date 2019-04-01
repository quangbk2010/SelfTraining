#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
//#pragma pack(1)
#define FOR_ALL_HELLO(i)\
	for(i = 1; i <= num_of_loop(); i ++)\
	{ \
	
#define FOR_ALL_HELLO_END(i)}

#define DATALEN 10
#define gettid() syscall(__NR_gettid)

int num_of_loop()
{
	int n = 1000;
	return 1000;
}

typedef struct __attribute__((__packed__)) student_s
{
	int id;        // sizeof (int) = 4
	char name[7];  // sizeof (name) = 7 + padding = 1 -> 8
} student_t;       // sizeof (student_t) = 12, with padding = 1 -> guarantee that its number % 4 == 0 (size of word = 4)

typedef struct book_s
{
	char name [30];
	int ssn;
	int page;
} book_t;

typedef struct data_s
{
	int a,b,c;
} data_t;

typedef struct mystruct_s
{
	char c;
	short s;
	int *i;
} mystruct_t;

void nhi_phan (float n) {
	int i = 0, *temp = (int *) (void *) &n, length = 8 * sizeof (n);
	for (i = 0; i < length; i++)
	{
		printf("%d", *temp >= 0);
		(*temp) <<= 1;
	}
	printf("\n");
}
void dinh_dang_ten () {
	char *name, *temp_name;
	name = (char *) malloc (20 * sizeof (char));
	printf("Nhap ten: ");
	scanf ("%[a-z ]", name);
	//gets (name);
	temp_name = name;
	int i, length, j;
	length = strlen (temp_name);
	printf("Length: %d\n", length);
	printf("\nSau dinh dang: ");
	for (i = length - 1; i >= 0; i --) {
		if ((i > 0 && *(temp_name + i - 1) == ' ') || i == 0) {
			//printf("i = %d\n", i);
			putchar (toupper (*(temp_name + i)));
			j = i + 1;
			while (*(temp_name + j)) {
				printf("%c", *(temp_name + j));
				j ++;
			}
			printf(" ");
			*(temp_name + i - 1) = '\0';
		}
	}
	
	printf("\n");
	free (name);
}
void earse_char () {
	char s[10] = "aaabcdaaf", c = 'a';
	int i = 0, j = 0;
	while (*(s+i)) {
		if (*(s+i) == c) {
			while (*(s+i+j)) {
				*(s+i+j) = *(s+i+j+1);
				j ++; 
			}
			j = 0;
		}
		else
			i ++;
	}
	printf("Chuoi sau chinh sua: %s\n", s);
}
void insert_char_k () {
	char s[10] = "abdefg", c = 'c';
	int k = 3;
	int i;
	char *temp = s;
	int length = strlen (temp);
	printf("%d\n", length);

	for (i = length - 1; i >= 0; i --) {
		*(temp + i + 1) = *(temp + i);
		if (i == k - 1) {
			*(temp + i) = c;
			break;
		}
		
	}
	printf("Chuoi sau them: %s\n", temp);
}
void count_number_word () {
	char s[30] = "    tong  hung    son trang ";
	int i = 0, n = 1, flag = 0;
	while (*(s+i)) {
		if (flag == 0) {
			if (*(s+i) != ' ')
				flag = 1;
		}
		else if (*(s+i) == ' ' && *(s+i+1) != ' ' && *(s+i+1) != '\0' && i > 0 ) {
			n ++;
		}
		i ++;
	}
	printf("number of word: %d\n", n);
}
void count_number_digit () {
	int n = 12345, length;
	char *s;
	s = (char *) malloc (10 * sizeof (char));
	sprintf (s, "%d", n);
	length = strlen (s);
	printf("the number of digit in %d is: %d\n", n, length);
}
void spec_format_pointer () {
	int n = 1;
	printf("The addr of n: %p %u %x\n", &n, n, n);
}
void size_of_struct () {
	//book_t b = {"Let Us C", 1000, 90} ;
	printf("Size of struct: %d\n", (int) sizeof (mystruct_t));
}
void size_of_pointer ()
{
	char *s, arr[10] = "bring";
	s = (char *) malloc (10);
	
	printf("\n%p %p %p %s %c %d\n", &s, s, (void *) s, s, *s, (int) sizeof (s));
	
	//s = "abc";
	strcpy (s, "abc");
	printf("\n%p %p %p %s %c %d %d %d %d\n", &s, s, (void *) s, s, *s, (int) sizeof (s), (int) strlen (s), (int) sizeof (arr), (int) strlen (arr));
	free (s);
}
void test_macro () {
	int i = 0;
	const char heart[] = "\xe2\x99\xa5";
	FOR_ALL_HELLO(i);
		printf("I %s you\n", heart);
		sleep (1);
	FOR_ALL_HELLO_END(i);
}
void handle_binary_file () {
	FILE *in, *out_txt, *out_bin;
	char s[] = "12345678";//\n2 binh";
	char *temp;
	int i;
	data_t data;
	temp = (char *) malloc (DATALEN * sizeof (char));

	/* Write string s into file test.txt */
	in = fopen ("test.txt", "w");
	fwrite (s, strlen(s), sizeof (char), in);
	fclose (in);

	/* Copy content of test.txt into file test2.txt and .bin to compare size of these files */
	in = fopen ("test.txt", "r");
	out_txt = fopen ("test2.txt", "w");
	out_bin = fopen ("test2.bin", "w");
	fread (temp, DATALEN, sizeof (char), in);
	printf("\nData in test.txt:\n%s\n", temp);
	fwrite (temp, strlen (temp) + 1, sizeof (char), out_txt);
	fwrite (temp, strlen (temp) + 1, sizeof (char), out_bin);
	fclose (in);
	fclose (out_txt);
	fclose (out_bin);

	/* Copy content of test2.bin, transfer to digits from hexa number, into file test3.txt */
	in = fopen ("test2.bin", "r");
	out_txt = fopen ("test3.txt", "w");
	fread (temp, DATALEN, 1, in);
	printf("\nData in test2.bin:\n%s\n", temp);
	printf("\nCharacters stored in test2.bin\n");
	for (i = 0; i < strlen (temp) + 1; i ++) {
		printf("%c\t", temp[i]);
	}
	fwrite (temp, strlen (temp), 1, out_txt);
	fclose (in);
	fclose (out_txt);

	/* Print with different format */
	printf("\nContent of test2.bin in hexa format\n");
	in = fopen ("test2.bin", "r");
	fread (temp, DATALEN, 1, in);
	for (i = 0; i < strlen (temp); i ++)
	{
		printf("%x\t", temp[i]);
	}
	printf("\n");
	fclose (in);

	/* tranfer content of test.txt from digits to hexa number */
	in = fopen ("test.txt", "a");
	fwrite ("\0", 1, 1, in);
	fclose (in);

	/* read a tructure from file */
	/* Write string s into file test.txt */
	in = fopen ("test.txt", "w");
	for (i = 0; i < DATALEN; i ++) {
		data.a = i;
		data.b = i * 2 - 1;
		data.c = i * 3;
		fwrite (&data, sizeof (data_t), 1, in);
	}
	fclose (in);

	in = fopen ("test.txt", "r");
	if (!in) {
		perror ("Unable to open file!");
		exit (1);
	}
	fseek (in, sizeof (data_t) * i, SEEK_END);
	rewind (in);
	printf("\nRead a tructure from file test.txt\n");
	printf("Size of data_t: %d\n", (int) sizeof (data_t));
	for (i = 0; i < DATALEN; i ++) {
		fread (&data, sizeof (data_t), 1, in);
		printf("%d %d %d\n", data.a, data.b, data.c);
	}
	fclose (in);

}
void test_strcpy() {
	char s[7];
	int i;
	printf("Thread ID: %d\n", gettid());
	strcpy (s, "abcdef");
	printf("%.*s %d %d\n", 3, s, (int) sizeof (s), (int) strlen (s));

	strncpy (s, "ab", 7);
	printf("%s %d %d\n", s, (int) sizeof (s), (int) strlen (s));
	for (i = 0; i < 10; i ++) {
		printf("%c", *(s + i));
	}
	printf("!\n");

	memset (s, 0, 7);
	strncpy (s, "ab", 7);
	printf("%s %d %d\n", s, (int) sizeof (s), (int) strlen (s));

	for (i = 0; i < 10; i ++) {
		printf("%c", *(s + i));
	}
	printf("!\n");
}
int cal (float x, char c) {
	return (x-c);
}
void ep_kieu () {
	int i;
	for (i = 1; i < 3; i ++) {
		void *p = &i;
		printf("%d\n", *(int *)(float *) p);
		//printf("%.50f\n", i);
	}
}
void check_allocate_function () {
	int *p = (int *) malloc (10* sizeof (int));
	int *p2 = (int *) malloc (10* sizeof (int));
	printf("%p\n", p);
	p = (int *) realloc (p, 20 * sizeof (int));
	int a[4] = {1,2,3,4};
	printf("%p\n", p);
	/*p = a;
	while (*p) {
		printf("%d\n", *p);
		p ++;
	}*/
	free (p);
	free (p2);
}
int main()
{
	check_allocate_function ();
	
	return 0;
}


