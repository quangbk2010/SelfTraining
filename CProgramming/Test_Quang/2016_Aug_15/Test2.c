#include <stdio.h>
#define N 10

int main(int argc, char const *argv[])
{
	FILE *fp;
	int i, temp[N], temp2[N], count = 0;
	fp = fopen ("data.txt", "wt");

	printf("Entering and write to data.txt, with %d number\n", N);
	for (i = 0; i < N; i ++)
	{
		scanf ("%d", temp + i);
	}

	for (i = 0; i < N; i ++)
	{
		fprintf(fp,"%3d\n",temp[i]);
	}
	fclose (fp);
	printf("Close data.txt\n");

	fp = fopen ("data.txt", "rt");
	printf("Read data from data.txt, and write to array\n", N);
	for (i = 0; i < N; i ++)
	{
		fscanf(fp,"%d",&temp2[i]);
	}

	for (i = 0; i < N; i ++)
	{
		if (!(temp2[i] % 2)) {
			printf("%d\t",temp2[i]);
			count ++;
		}
	}
	printf("\nThe number of even: %d\n", count);
	fclose (fp);
	printf("\nClose data.txt\n");


	return 0;
}