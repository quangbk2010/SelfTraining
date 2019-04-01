#ifndef _SERVER_H
#define _SERVER_H

#include "util.h"
#include "unprtt.h"
#include <signal.h>
#include <setjmp.h>


int initilaize_server_tcb(tcb *sertcb,uint32_t cliwin,uint32_t servwin);
void sendFin(tcb *clitcb,int fd);
//int transmit_data(tcb *ser_tcb,int *sock_fd,int filefd,struct rtt_info rttInfo,tcb *sertcb);
int transmit_data(int *sock_fd,int filefd,struct rtt_info rttInfo,tcb *sertcb);

/* void signal_transmit_file(int sigNum);
void signal_conn_close(int sigNum);
void send_probe(tcb *sertcb,int fd);
pckQueue *update_tcb_parameters(tcb *tcb,pack_struct *packet,int size);
void increse_congestion_window(tcb *tcb,int ackflag);
void additive_ack_increment(tcb *tcb);
void reset_nodes(tcb *tcb);
void sendFin(tcb *clitcb,int fd);
*/
#endif
