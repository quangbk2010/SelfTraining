#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1
timer_t timerid;


static void handler(int sig, siginfo_t *si, void *uc)
{
    if(si->si_value.sival_ptr != &timerid){
        printf("Stray signal\n");
    } else {
        printf("Caught signal %d from timer\n", sig);
        system (" paplay /usr/share/sounds/freedesktop/stereo/complete.oga");
    }
}

int main(int argc, char *argv[])
{
    struct sigevent sev;
    struct itimerspec its;
    long long freq_nanosecs;
    sigset_t mask;
    struct sigaction sa;

    printf("Establishing handler for signal %d\n", SIG);
    sa.sa_flags = SA_SIGINFO;  // sa_flags specifies a set of flags which modify the behavior of the signal
    sa.sa_sigaction = handler; // If SA_SIGINFO is specified in sa_flags, then sa_sigaction (instead of sa_handler) specifies the signal-handling function
    sigemptyset(&sa.sa_mask);  // The signal set sa_mask is the set of signals that are blocked when the signal handler is being executed
    sigaction(SIG, &sa, NULL);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    timer_create(CLOCKID, &sev, &timerid);
    /* Start the timer */

    its.it_value.tv_sec = 10;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    timer_settime(timerid, 0, &its, NULL);
    sleep(100);
    exit(EXIT_SUCCESS);
}
/*

    sa.sa_flags = SA_SIGINFO;  // sa_flags specifies a set of flags which modify the behavior of the signal, read sigaction struct for more details.
    sa.sa_sigaction = handler; // If SA_SIGINFO is specified in sa_flags, then sa_sigaction (instead of sa_handler) specifies the signal-handling function
    sigemptyset(&sa.sa_mask);  // The signal set sa_mask is the set of signals that are blocked when the signal handler is being executed
							   // or: a set of signals that aren’t permitted to interrupt execution of this handler. 
							   // In addition, the signal that caused the handler to be invoked is automatically added to the process signal mask.
							   // This function (sigemptyset) will set this set of signal are empty.
    sigaction(SIG, &sa, NULL);
							   // can reuse the struct (changing what you want to change). The system reads all information that it needs and forgets 
							   // about it so in fact you can even destroy the struct without problems 
							   // (and that is what happens when you go out of scope, as it's an automatic variable).
*/