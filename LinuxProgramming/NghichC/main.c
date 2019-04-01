#include <stdio.h>
#include <stdlib.h>

int add (char *a, int n) {
	int i, s = 0;
	for (i = 0; i < n; i ++)
		s += (int) a[i];
	return s;
}

void init (char *a, int n) {
	int i;
	for (i = 0; i < n; i ++) {
		a[i] = 0; // off
	}
}

void print (int k, int s, char *a, int n) {
	printf("Result: %d, sum = %d\n", k, s);
	int i;
	for (i = 0; i < n; i ++) {
		printf ("a[%d] = %d\t", i, a[i]); // off
	}
	printf("\n");
}

void func (char *x, int n) {
	int i, j, s;

	init (x, n);
	for (j = 1; j < 100; j ++) {
		
		if (j % 4 == 1)
			for (i = 0; i < n; i ++) {
				if ((i + 1) % 2 == 0) 
					x[i] =~ x[i];
			}
		else if (j % 4 == 2)
			for (i = 0; i < n; i ++) {
				if ((i + 1) % 3 == 0) 
					x[i] =~ x[i];
			}
		else if (j % 4 == 3)
			for (i = 0; i < n; i ++) {
				if ((i + 1) % 4 == 0) 
					x[i] =~ x[i];
			}
		else {//if (j % 4 == 0) 
			x[0] =~ x[0];
			x[4] =~ x[4];
			x[6] =~ x[6];
		}
		printf ("j = %d\n", j);
		s = add (x, n);
		//print (j, s, x, n);
		if (j > 1 && (s == 0 || s == -n)) {
			//break;
			print (j, s, x, n);
			//exit (1);
		}
	}
}
int main () {
	char a[8];
	init (a, 8);
	func (a, 8);
	return 0;
}