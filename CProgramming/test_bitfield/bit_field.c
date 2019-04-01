#include <stdio.h>

typedef struct 
{
	unsigned int a : 1;
	unsigned int b : 1;
} status1_t;

typedef struct 
{
	unsigned int a1 : 20;
	unsigned int a2 : 20;
	//unsigned int a3 : 20;
} status2_t;

typedef struct 
{
	char a : 1;
	char b : 1;
} status3_t;

typedef struct 
{
	int a : 8;
	int b : 6;
	int c : 8;
	int d : 2;
} status4_t;

typedef struct 
{
	short a : 8;
	short b : 6;
	short c : 8;
	short d : 2;
} status5_t;

typedef struct 
{
	int a : 8;
	int b : 6;
	int c : 8;
	int d;
} status6_t;

typedef struct 
{
	int a : 1;  // First 4 bytes are allocated
	int b : 31; // Will get accomodated in the First 4 bytes
	int c : 1;  // Second 4 bytes are allocated
} status7_t;

typedef struct 
{
	int a : 1;  // First 4 bytes are allocated
	int b : 32; // Will NOT get accomodated in the First 4 bytes, Second 4 bytes are allocated
	int c : 1;  // Will NOT get accomodated in the Second 4 bytes, Third 4 bytes are allocated
} status8_t;

typedef struct 
{
	int a : 32; // First 4 bytes are allocated
	int b : 1;  // Second 4 bytes are allocated
	int c : 1;  // Will get accomodated in the First 4 bytes
} status9_t;

union char_and_ascii
{
    char ch;
    unsigned short ascii_val;
};

int main(int argc, char const *argv[])
{

	printf("Sizeof a struct: %u %u %d %u %u %u %u %u %u\n", (int) sizeof (status1_t), (int) sizeof (status2_t), (int) sizeof (status3_t), (int) sizeof (status4_t), (int) sizeof (status5_t), (int) sizeof (status6_t), (int) sizeof (status7_t), (int) sizeof (status8_t), (int) sizeof (status9_t) );
	
	union char_and_ascii obj;
    obj.ascii_val = 0;
    obj.ch = 'A';
    printf("\n character = [%c], ascii_value = [%u]\n", obj.ch, obj.ascii_val);

	return 0;
}