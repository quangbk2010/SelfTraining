#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_t thread1, thread2, thread3;
pthread_mutex_t mutex;
int a[100];
struct sched_param param, param_set;
int policy, retcode;

void *function1 (void *ptr) {
    int i;
    char *msg = (char *) ptr;
    char *c;

    if (msg == "thread1")
        c = "X ";
    else
        c = "A ";
    //pthread_mutex_lock(&mutex);
    printf("\n%s\n", msg);

    for (i = 0; i < 10; i ++) {
        printf("%s", c);
        a[i] = i;
        sleep (1);
        
    }
    printf("\n");
    //pthread_mutex_unlock(&mutex);
}

void *function2 (void *ptr) {
    int i;
    char *msg = (char *) ptr;
    struct sched_param param;
    int policy, retcode;

    //pthread_mutex_lock(&mutex);

    printf("\n%s\n", msg);
    for (i = 0; i < 100; i ++) {
        printf("%d ", a[i]);
        //usleep (1);
    }
    printf("\n");
    //pthread_mutex_unlock(&mutex);
}
int main(void)
{
    char *msg1 = "thread1", *msg2 = "thread2", *msg3 = "thread3";
    memset (a, 0, sizeof(a));
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread3, NULL, function1, (void *) msg3);
    pthread_create(&thread1, NULL, function1, (void *) msg1);
    int j;
    //pthread_create(&thread2, NULL, function2, (void *) msg2);
    printf("a\n");
    //pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("b\n");
    pthread_join(thread1, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}

    /*param_set.sched_priority = 10;
    retcode = pthread_getschedparam( pthread_self(), &policy, &param);
    pthread_setschedparam( pthread_self(), SCHED_FIFO , &param_set);

    retcode = pthread_getschedparam( pthread_self(), &policy, &param);
    printf("\n%s: Priority: %d\n", msg, param.sched_priority);*/