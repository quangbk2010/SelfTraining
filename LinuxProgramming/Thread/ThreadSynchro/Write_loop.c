#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex;
int a[100];
void *function1 (void *ptr) {
    int i;
    char *msg = (char *) ptr;

    //pthread_mutex_lock(&mutex);
    printf("\n%s\n", msg);

    for (i = 0; i < 100; i ++) {
        a[i] = i;
        usleep (1);
    }
    printf("\n");
    //pthread_mutex_unlock(&mutex);
}

void *function2 (void *ptr) {
    int i;
    char *msg = (char *) ptr;

    //pthread_mutex_lock(&mutex);
    printf("\n%s\n", msg);

    for (i = 0; i < 100; i ++) {
        printf("%d ", a[i]);
        usleep (1);
    }
    printf("\n");
    //pthread_mutex_unlock(&mutex);
}
int main(void)
{
    pthread_t thread1, thread2;
    char *msg1 = "thread1", *msg2 = "thread2";
    memset (a, 0, sizeof(a));
    //pthread_mutex_init(&mutex, NULL);
    //printf("XXXXXXX\n");

    pthread_create(&thread1, NULL, function1, (void *) msg1);
    pthread_create(&thread2, NULL, function2, (void *) msg2);
    //printf("AAAAAAA\n\n");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    //pthread_mutex_destroy(&mutex);
    return 0;
}