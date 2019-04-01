/*#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>


#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1//SIGINT//
timer_t timerid;


static void handler(int sig, siginfo_t *si, void *uc)
{
    // should not use printf in signal handler 
    // -> use 1.flag or 2.use write(STDOUT_FILENO, ...) or 3.make sound
    // a list of async-signal-safe functions: http://man7.org/linux/man-pages/man7/signal.7.html
    if(si->si_value.sival_ptr != &timerid){
        system ("aplay /usr/share/sounds/alsa/Side_Right.wav");
        printf("Error\n");
        exit (EXIT_FAILURE);
    } else {
        system (" paplay /usr/share/sounds/freedesktop/stereo/complete.oga");
        printf("Sucess\n");
        exit(EXIT_SUCCESS);
    }
}

// init sigaction 
void init_sigaction() { 
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;  // sa_flags specifies a set of flags which modify the behavior of the signal
    sa.sa_sigaction = handler; // If SA_SIGINFO is specified in sa_flags, then sa_sigaction (instead of sa_handler) specifies the signal-handling function
    sigemptyset(&sa.sa_mask);  // The signal set sa_mask is the set of signals that are blocked when the signal handler is being executed
    sigaction(SIG, &sa, NULL); // CHECK pages 521> IN "BEGINNING LINUX PROGRAMMING"

}

// create timer
void create_timer () {
    struct sigevent sev;
    struct itimerspec its;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    timer_create(CLOCKID, &sev, &timerid);
    
    // Start the timer 
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    timer_settime(timerid, 0, &its, NULL);
}

int main(int argc, char *argv[])
{
    
    printf("Establishing handler for signal %d\n", SIG);
    init_sigaction ();

    create_timer();
    sleep(5);
    exit(EXIT_SUCCESS);
}*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include <stdlib.h>

#include <time.h>

int count = 0;
#define SIG SIGPROF
#define CLOCKID CLOCK_REALTIME

// signal process 
void timeout_info(int signo)
{
   count ++;
   printf("count = %d\n", count);
   if (count == 3) {
        printf("Exit.\n");
        exit (EXIT_FAILURE);
   }
   sleep (1);
}

// init sigaction 
void init_sigaction(void)
{
    struct sigaction act;

    act.sa_handler = timeout_info;
    act.sa_flags   = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIG, &act, NULL);
} 

// init
void init_time(void)
{
    struct itimerval val;
    struct sigevent sev;
    struct itimerspec its;
    sigset_t mask;
    timer_t timerid;

    // First way: create timer
    //val.it_value.tv_sec = 1;
    //val.it_value.tv_usec = 0;
    //val.it_interval = val.it_value;
    //setitimer(ITIMER_PROF, &val, NULL);

    // second way
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    timer_create(CLOCKID, &sev, &timerid);
    
    // Start the timer 
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    timer_settime(timerid, 0, &its, NULL);
}


int main(int argc, char *argv[])
{
    char *str;
    char c;

    init_sigaction();
    init_time();
    while(1);
    exit(EXIT_SUCCESS);
}