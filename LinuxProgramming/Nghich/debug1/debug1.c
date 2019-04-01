#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char *data;
	int key;
} item;
item array[] = 		{
	{"bill", 3},
	{"neil", 4},
	{"john", 2},
	{"rick", 5},
	{"alex", 1}
};

void sort(item *a, int n) {
	int i = 0, j = 0;
	int s = 1;
	for(; i < n && s != 0; i++) {
		s = 0;
		for(j = n-1; j > 0; j--) {
			if(a[j].key < a[j-1].key) {
				item t = a[j];
				a[j] = a[j-1];
				a[j-1] = t;
				s++;
			}
		}
		n--;
	}
}
void main( int argc, char *argv[])
{
	int i, j;
	sort(array,5);
	for (i = 0; i < 5; i ++)
		printf("a[%d] = %d\t", i, array[i].key);
	/*char *str = argv[1];
	while (1) {
		sleep (1);
		printf ("%s\n", str);
	}*/
}
