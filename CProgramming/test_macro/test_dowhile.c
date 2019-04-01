#include <stdio.h>
#define DEBUG 0

#define debug_print(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
int main(int argc, char const *argv[])
{
	debug_print("test: %s\n", "macro");
	return 0;
}