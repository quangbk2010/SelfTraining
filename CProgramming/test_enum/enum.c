#include <stdio.h>

typedef enum boolean_s {TRUE, FALSE} boolean_t;
enum day {mon = 2, tue, wed, thu, fri, sat, sun};

typedef enum {jan2 = 1, feb2, mar2, apr2} month2;

boolean_t equal (int m, int n) {
	if (m == n)
		return TRUE;
	else
		return FALSE;
}

enum day day_of_week (int d) {
	switch (d) {
		case 2: return mon; break;
		case 3: return tue; break;
		case 4: return wed; break;
		case 5: return thu; break;
		case 6: return fri; break;
		case 7: return sat; break;
		case 8: return sun; break;
		default: return -1;
	}
}

int main(int argc, char const *argv[])
{
	enum {jan = 1, feb, mar, apr} month;
	int a = 1, b = 2;
	boolean_t check = equal (a++, b);
	printf("check = %d\n", check);

	a --;
	check = equal (++a, b);
	printf("check = %d\n", check);

	enum day d;
	a = 1;
	d = day_of_week (a);
	printf("day = %d\n", d);

	d = day_of_week (6);
	printf("day = %d\n", d);	

	month = 10;
	month2 m2;
	//m = jan;
	m2 = jan2;
	int temp = m2;
	m2 = 5;
	printf("month = %d %d %d \n", month, m2, temp);
	printf("c_month = %c %c %c %c %c %c\n", month, m2, month, m2, temp, temp);
	return 0;
}
