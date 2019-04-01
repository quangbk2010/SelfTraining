#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_t thread1, thread2;
pthread_mutex_t mutex;
int a[100];
struct sched_param param, param_set;
int policy, retcode;

void *function1 (void *ptr) {
    int i;
    char *msg = (char *) ptr;

    printf("\n%s\n", msg);

    for (i = 0; i < 100; i ++) {
        printf("X ");
        a[i] = i;
        usleep (1); // IF REMOVE => RESULTS IS OK
        
    }
    printf("\n");
}

void *function2 (void *ptr) {
    int i;
    char *msg = (char *) ptr;
    struct sched_param param;
    int policy, retcode;

    printf("\n%s\n", msg);
    for (i = 0; i < 100; i ++) {
        printf("%d ", a[i]);
        usleep (1);
    }
    printf("\n");
}
int main(void)
{
    char *msg1 = "thread1", *msg2 = "thread2";
    memset (a, 0, sizeof(a));
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread2, NULL, function2, (void *) msg2);
    pthread_create(&thread1, NULL, function1, (void *) msg1);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
