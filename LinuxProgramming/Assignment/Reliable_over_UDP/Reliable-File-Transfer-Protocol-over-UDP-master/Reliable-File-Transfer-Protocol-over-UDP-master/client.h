#ifndef _CLIENT_H
#define _CLIENT_H

extern int seed;
extern float lossProbability;
extern int exp_distributor;

void get_client_parameters();
struct in_addr *get_host(char *host, char *addr);
int check_server_assign_client(struct in_addr *ipserver,struct in_addr *ipclient);
int initialize_client(struct sockaddr_in *ipserver,struct in_addr client_addr,int flag_dont_route);
int receiveFile(int *sckfd,int cliwindow,tcb *clitcb);
int initilaize_client_tcb(tcb *clitcb,uint32_t cliwin,uint32_t servwin);
int receive_file(int *sockfd,int winsize,tcb *clitcb);
static int update_tcb_parameters(tcb *clitcb,pack_struct *packet,int size);
int isPacketLost();
void send_acks(tcb *clitcb, int fd);
void send_probe_acks(tcb *clitcb,int fd);
static void sendFinAck(tcb *clitcb,int fd);

#endif
