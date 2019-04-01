#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int counter;
pthread_mutex_t mutex ;//= PTHREAD_MUTEX_INITIALIZER;

void* doSomeThing()
{
    unsigned long i = 0;
    pthread_mutex_lock (&mutex);
    counter += 1;

    printf("\n Job %d started\n", counter);

    //for(i=0; i<(0xFFFFFFF);i++);
    sleep (10);
        printf("\n Job %d finished\n", counter);
    pthread_mutex_unlock (&mutex);
}

int main(void)
{
    int i = 0;
    int err;

    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}