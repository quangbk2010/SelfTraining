#include <stdio.h>
#define f(a, b) a*b;
void main () {
    int a = 8, b = 1;
    b = f (a++, ++b);
    printf ("%d\t%d\n",a, b);
}
