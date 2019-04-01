#include <stdio.h>    
#include <math.h>

void list_prime (int n, int arr[], int *len) {
    int i, j, k = 0, count = 0;
    for (i = 2; i <= n;i ++)
    {
        for (j = 1; j <= i; j ++)
        {
            if(i % j == 0)
                count = count + 1;
        }
        if(count == 2) {
            arr[k] = i;
            k ++;
        }
        count=0;
    }
    *len = k;
}
void print_list (int arr[], int len) {
    int i;
    printf("Length of arr: %d\n", len);
    for (i = 0; i < len; ++i)
    {
        printf("%d\t", arr[i]);
    }
}
int find_list (int arr[], int begin, int end, int compare_number) {
    int i;
    for (i = begin; i < end; i ++) {
        if (arr[i] == compare_number)
            return i;
    }
}
int k_prime (int n, int k) {
    int arr[n], len, i, j = 0, temp;
    list_prime (n, arr, &len);
    //print_list (arr, len);
    int big_arr[(int)(len/3)];
    temp = n/3;
    int threshold = find_list (arr, 0, len, temp);
    for (i = threshold; i < len; i ++) {
        big_arr[j] = arr[i];
        j ++
    }
    
    return 0;
}
int main(int argc, char const *argv[])
{
    int len, arr[53];// = {1,2,3,4,5};
    k_prime (5, 1);
    return 0;
}
