#include <stdio.h>
#define foo(x, y) (x / y)
#define f1(m, n) #m #n
int main()l
	    {
	        int i = -6, j = 3;
	        char buf[20], temp[30];
	        //FILE *fp;
	        //printf("%d\n",foo((i + j), 3));
	        //printf("%s\n", f1(k, l));
	        printf("%d\n", getpid());
	        for (i = 0; i < 10000000; i ++)
	        	fprintf(stderr, "%s", "abc");
	        read (2, (void *) buf, 20);

	        //fp = fopen("stderr", "r");
	        //fscanf (fp, "%s", buf);
	        //fclose (fp);
	    }
