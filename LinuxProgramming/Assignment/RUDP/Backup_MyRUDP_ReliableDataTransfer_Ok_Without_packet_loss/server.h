#ifndef _SERVER_H
#define _SERVER_H

#include <setjmp.h>

#define SERVER_INPUT            "server.in"
#define MAXSIZE                 20            // size of character array used to read file
#define IP_LEN                  16
#define MAX_FILENAME_LEN        32

#define FILE_CHUNK_SIZE         (1024 * 1024) // size of chunk which will be sent to the client
#define BUF_SIZE                (1024 * 1024)
#define HOST                    "SERVER"
#define PEER_HOST          		"CLIENT"

int serverPort;
int swnd;
char req_file_name[MAX_FILENAME_LEN];

//sigjmp_buf hello_signal;

/*static int read_server_params ();
static int init_server ();//struct sockaddr_in server_addr);
static void serHandShake(int sockfd, struct sockaddr *addr, int addLen);//, pckStruct_t pck_sent, pckStruct_t pck_rcvd);
static void signal_hello_function(int sigNum);
*/
/*
struct ifi_info {
  char    ifi_name[IFI_NAME];	// interface name, null terminated 
  u_char  ifi_haddr[IFI_HADDR];	// hardware address 
  u_short ifi_hlen;				// #bytes in hardware address: 0, 6, 8 
  short   ifi_flags;			// IFF_xxx constants from <net/if.h> 
  short   ifi_myflags;			// our own IFI_xxx flags 
  struct sockaddr  *ifi_addr;	// primary address 
  struct sockaddr  *ifi_brdaddr;// broadcast address 
  struct sockaddr  *ifi_dstaddr;// destination address 
  struct ifi_info  *ifi_next;	// next of these structures 
};
*/

int pck_rcvd_rwnd;

/* Timeout flag */
int isTimeout = 0, try = 0;

#endif