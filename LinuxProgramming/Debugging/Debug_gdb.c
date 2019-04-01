#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char *data;
	int key;
} item;
item array[] = {
	{"bill", 3},
	{"neil", 4},
	{"john", 2},
	{"rick", 5},
	{"alex", 1},
};
sort(item *a, int n) {
	int i = 0, j = 0;
	int s = 1;
	for(; i < n && s != 0; i++) {
		s = 0;
		for(j = n - 1; j > 0; j--) { 
			if(a[j].key > a[j-1].key) { // <
				item t = a[j];
				a[j] = a[j-1];
				a[j-1] = t;
				s++; // s = 1;
			}
			//printf("\nTime: " __TIME__ "\nDate: " __DATE__ "\n" );
			//printf("Line: %d\nFile: %s\n", __LINE__, __FILE__ );
		}
		n--;
	}
}
void main()
{
	int i;
	sort(array,5);
	for(i = 0; i < 5; i++)
		printf("array[%d] = {%s, %d}\n",i, array[i].data, array[i].key);
}
/*
1. ./Makefile // we can lauch gdb in there
2. break 20
3. run
4. p a[0] @ 5
5. c
6. p a[0] @ 5
*/