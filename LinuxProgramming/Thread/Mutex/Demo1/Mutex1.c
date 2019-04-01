#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *functionC();
void *functionP();
pthread_t thread1, thread2, thread3;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  i = 0, j = 0, sum;

main()
{
   int rc1, rc2, rc3;
   //char *message1 = "Thread1", *message2 = "Thread2";

   /* Create independent threads each of which will execute functionC */

   //if( (rc1=pthread_create( &thread1, NULL, &functionC, (void *) message1)) )
   if( (rc1=pthread_create( &thread1, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }

   if( (rc2=pthread_create( &thread2, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc2);
   }

   if( (rc3=pthread_create( &thread3, NULL, &functionP, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc3);
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL); 
   pthread_join( thread3, NULL);

   exit(0);
}

//void *functionC(void *ptr)
void *functionC()
{
   char *message;
   int t;
   //message = (char *) ptr;
   pthread_t id = pthread_self ();

   while (i < 10 && j < 10) {
      //pthread_mutex_lock( &mutex1 );
      if (pthread_equal(id, thread1)) {
         i ++;
         t = i;
         message = "Thread1: i = ";
      }
      if (pthread_equal(id, thread2)) {
         j ++;
         t= j;
         message = "Thread2: j = ";  
      }
      printf("%s%d\n", message, t);
      //pthread_mutex_unlock( &mutex1 );
      sleep (1);
   }
}

void *functionP()//void *ptr)
{
   char *message;
   //message = (char *) ptr;
   while (1) {
      //pthread_mutex_lock( &mutex1 );
      sum = i + j;
      printf("Thread3: calculate    sum: %d\n", sum);
      i += 3;
      //pthread_mutex_unlock( &mutex1 );
      sleep (1);
   }
}
