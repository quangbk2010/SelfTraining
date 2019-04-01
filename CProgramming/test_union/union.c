#include <stdio.h>

typedef union {
	unsigned int ival;
	float fval;
	unsigned char ch[2];
} val;

typedef union {
	int ival;
	float fval;
	char ch[2];
} val2;

typedef union {
	int ival;
	double fval;
	char ch[2];
} val3;

int main(int argc, char const *argv[])
{
	val a;
	val2 b;
	val3 c;

	printf("\nAssign with int value\n");
	a.ival = 0xa1b2;
	b.ival = 0xa1b2;
	c.ival = 0xa1b2;

	printf("%x %d\n", a.ch[0], a.ch[0]);
	printf("%x %d\n\n", a.ch[1], a.ch[1]);

	printf("%x %d\n", b.ch[0], b.ch[0]);
	printf("%x %d\n\n", b.ch[1], b.ch[1]);

	printf("%x %d\n", c.ch[0], c.ch[0]);
	printf("%x %d\n", c.ch[1], c.ch[1]);

	printf("\nAssign with float value\n");
	a.fval = 13;//0xa1b2;
	b.fval = 131;//0xa1b2;
	c.fval = 132;//0xa1b2;

	printf("%x %d\n", a.ch[0], a.ch[0]);
	printf("%x %d\n\n", a.ch[1], a.ch[1]);

	printf("%x %d\n", b.ch[0], b.ch[0]);
	printf("%x %d\n\n", b.ch[1], b.ch[1]);

	printf("%x %d\n", c.ch[0], c.ch[0]);
	printf("%x %d\n\n", c.ch[1], c.ch[1]);
	return 0;
}