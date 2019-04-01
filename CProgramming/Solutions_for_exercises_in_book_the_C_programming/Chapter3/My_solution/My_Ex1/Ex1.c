#include <stdio.h>
#include <stdlib.h>

int binsearch (int x, int v[], int n, int *count) {
    int low, high, mid, temp = 0;

    low = 0; 
    high = n - 1;
    mid = (low + high) / 2;
    while (low <= high && x != v[mid]) {
        if (x < v[mid])
            high = mid - 1;
        else
            low = mid + 1;
        mid = (low + high) / 2;
        temp ++;
    }
    *count = temp;
    //printf ("low = %d, high = %d, count = %d\n", low, high, *count);
    if ( x == v [mid])
        return mid;
    else
        return -1;
}

int main (int argc, char *argv[]) {
    int arr[20] = {0, 1, 2, 3, 4};
    int n = 4, x, pos;
    int count;
    if (argc < 2) {
        fprintf (stderr, "Usage: [%s <numb>]\n", argv[0]);
        return 1;
    }
    x = atoi(argv[1]);
    printf ("Begining\n");
    pos = binsearch (x, arr, n, &count); 
    printf ("pos = %d, count = %d\n", pos, count);

    //printf ("pos = %d\n", pos);
    printf ("Finishing\n");
    return 0;
}
