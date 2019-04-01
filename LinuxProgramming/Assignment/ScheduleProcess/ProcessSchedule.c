/* Scheduling Simulation*/
     
#include <stdio.h>
#include <stdlib.h>
     
/* Process Data Structure */
typedef struct process {
	int pid; /* Process ID */
	int priority; /* Priority */
	int arrival_time /* Time when process submit request */;
	int burst_time; /* CPU burst Time, duration which processor need to implement a process */
    struct process *next;
} Process;

/* Function Prototype Declarations */
void fcfs (Process *proc);
Process *init_process (int pid, int burst_time, int arrival_time, int priority);
void list_process (Process *proc);
void priority (struct process *proc);

/* Main Function */
int main () {
	Process *plist, *ptmp;
	plist = init_process (1, 2, 0, 4);
	plist -> next = init_process (2, 3, 1, 1); ptmp = plist -> next;
	ptmp -> next = init_process (3, 4, 2, 3); ptmp = ptmp -> next;
	ptmp -> next = init_process (4, 5, 3, 2); ptmp = ptmp -> next;

	list_process (plist);
	fcfs (plist);
	priority (plist);
	return 0;
}

/* Process initiation function */
Process *init_process (int pid, int burst_time, int arrival_time, int priority) {
	Process *proc;
	proc = malloc (sizeof (Process));
	if (proc == NULL) {
		perror ("Memory allocation failure!");
	}
	proc -> pid = pid;
	proc -> burst_time = burst_time;
	proc -> arrival_time = arrival_time;
	proc -> priority = priority;
	proc -> next = NULL;

	return proc;
}

/* Print information about set of processes */
void list_process (Process *proc) {
	Process *ptmp;
	int pid, burst_time, arrival_time, priority;

	ptmp = proc;
	printf("Process list:\nPid\tArrival_time\tBurst_time\tPriority\n");
	
	while (ptmp != NULL) {
		pid = ptmp -> pid;
		burst_time = ptmp -> burst_time;
		arrival_time = ptmp -> arrival_time;
		priority = ptmp -> priority;

		printf("%2d\t%3d\t%12d%14d\n", pid, arrival_time, burst_time, priority);
		ptmp = ptmp -> next;
	}
}

/* First come First served scheduling algorithm */
void fcfs (Process *proc) {
	int start, duration = 0, end, turnaround_time, waiting_time, service_time = 0;
	Process *ptmp = proc;

	printf("\n\nFirst come First served scheduling algorithm\n");
	printf("Pid\tArrival_time\tBurst_time\tService_time\tTurnaround_time\tWaiting_time\tPriority\n");
	while (ptmp != NULL) {
		service_time = (service_time > ptmp -> arrival_time) ? service_time : ptmp -> arrival_time;
		turnaround_time = service_time + ptmp -> burst_time;
		waiting_time = service_time - ptmp -> arrival_time;

		printf("%2d\t%6d\t%12d\t%12d\t%12d\t%12d\t%14d\n", ptmp -> pid, ptmp -> arrival_time, ptmp -> burst_time, service_time, turnaround_time, waiting_time, ptmp -> priority );
		service_time += ptmp -> burst_time;
		
		ptmp = ptmp -> next;		
	}
	printf("END:\tFCFS scheduling simulation\n\n");
}

     
     
/* Priority scheduling simulation
* Note: lower priority value gets a higher priority
 */
void priority (struct process *proc) {
    int highest, turnaround_time, waiting_time, service_time = 0;
    struct process *copy, *tmpsrc, *tmp, *beforehighest;
     
    printf("\n\nPriority scheduling simulation\n");
	printf("Pid\tArrival_time\tBurst_time\tService_time\tTurnaround_time\tWaiting_time\tPriority\n");
     
    /* Duplicate process list */
    tmpsrc = proc;
    copy = tmp = NULL;
    while (tmpsrc != NULL) {
	    if (copy == NULL) {
		    copy = init_process(tmpsrc->pid, tmpsrc->burst_time, tmpsrc -> arrival_time, tmpsrc->priority);
		    tmp = copy;
	    } else {
		    tmp->next = init_process(tmpsrc->pid, tmpsrc->burst_time, tmpsrc -> arrival_time, tmpsrc->priority);
		    tmp = tmp->next;
	    }
	    tmpsrc = tmpsrc->next;
    };
     
    /* Main routine */
    while (copy != NULL) {
	    /* Find the next job */
	    beforehighest = NULL;
	    highest = copy->priority;
	    tmp = copy->next;
	    tmpsrc = copy;
		/* Find the process with highest priority, but remember the previous process  (in queue) */
	    while (tmp != NULL) {
		    if (tmp->priority < highest) {
			    highest = tmp->priority;
			    beforehighest = tmpsrc;
		    }
		    tmpsrc = tmp;
		    tmp = tmp->next;
	    }
	     
	    /* Process job and remove from copy of process list */
	    if (beforehighest == NULL) {
		    /* Handle first job (in queue) is highest priority case */
		    service_time = (service_time > copy -> arrival_time) ? service_time : copy -> arrival_time;
			turnaround_time = service_time + copy -> burst_time;
			waiting_time = service_time - copy -> arrival_time;
			
			printf("%2d\t%6d\t%12d\t%12d\t%12d\t%12d\t%14d\n", copy -> pid, copy -> arrival_time, copy -> burst_time, service_time, turnaround_time, waiting_time, copy -> priority );
			
			service_time += copy -> burst_time;
			
		    tmpsrc = copy->next;
		    free(copy);
		    copy = tmpsrc;
	    } else {
		    /* Handle first job is not highest priority case */
			/* Point to the highest priority process */
		    tmp = beforehighest->next;
			
		    service_time = (service_time > tmp -> arrival_time) ? service_time : tmp -> arrival_time;
			turnaround_time = service_time + tmp -> burst_time;
			waiting_time = service_time - tmp -> arrival_time;
			
			printf("%2d\t%6d\t%12d\t%12d\t%12d\t%12d\t%14d\n", tmp -> pid, tmp -> arrival_time, tmp -> burst_time, service_time, turnaround_time, waiting_time, tmp -> priority );
			
			service_time += tmp -> burst_time;
						
		    beforehighest->next = tmp->next;
		    free(tmp);
	    }
    }
     
    printf("END:\tPriority scheduling simulation\n\n");
}