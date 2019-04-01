#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char string[15];
	//strcpy (string, "192.168.0.1");
	int j, flag;
	for (j = 0; j < 10; j ++) {
		printf("Input IPv4 IP address: ");
		gets (string);
		int ip[4];
		char s[4][3], *temp;
		int i = 0;
		const char *dot = ".";

		int len = strlen (string);
		/*if (len > 15) 
			flag = 1;
		for (i = 0; i <  len; ++i)
		{
			if ( ((!isdigit(string[i]))) && string[i] != '.' ) {
				flag = 1;
				break;
			}
		}*/
		
		temp = strtok (string, dot);
		while (temp != NULL) {
			strcpy (s[i], temp);
			
			ip[i] = atoi (s[i]);
			i ++;
			temp = strtok (NULL, dot);
		}
		//printf("i = %d\n", i);
		if (i != 4)
			flag = 1;

		for (i = 0; i < 4; i++) {
			if (ip[i] > 255) {
				flag = 1;
				break;
			}
		}

		if (flag == 1) {
			continue;
		}

		printf("Your IP address is: \n");

		for (i = 0; i < 4; i++) {
			printf("%d", ip[i]);
			if (i < 3)
				printf(".");
		}
		printf("\n");
	}
	
	return 0;
}