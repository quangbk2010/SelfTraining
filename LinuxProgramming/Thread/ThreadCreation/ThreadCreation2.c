#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

void *print_message_function( void *ptr );
void *print_message_2_function();
void catchSignal(int sig);
int n = 0;
pthread_t thread1, thread2, thread3;

void main()
{
     
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2, iret3;

    /* Create independent threads each of which will execute function */

     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);
     iret3 = pthread_create ( &thread3, NULL, print_message_2_function, NULL);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
     pthread_join( thread3, NULL); 

     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
     printf("Thread 3 returns: %d\n",iret3);

     //exit(0);
}

void *print_message_function( void *ptr )
{
     char *message;
     message = (char *) ptr;
     while (1) {
          if (n >= 11) {
               printf("%s exit.\n", message);
               pthread_exit (NULL); // => check the signal existence when terminate the thread that use this function 
          }
          n ++;
          printf("%s: n = %d\n", message, n);
          sleep (1);
     }
}

void *print_message_2_function() 
{
     
     (void) signal (SIGINT, catchSignal);
     while (1) {
          if (n >= 7) {
               printf("Thread 3 exit.\n");
               pthread_exit (NULL); // => check the signal existence when terminate the thread that use this function 
          }
          n ++;
          printf ("Thread 3: n = %d.\n", n);
          sleep (1);
     }
}

void catchSignal (int sig) {
     printf ("Ohh, I got signal %d\n", sig);
     (void) signal (SIGINT, SIG_DFL);
}


