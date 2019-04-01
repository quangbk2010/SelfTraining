#include "util.h"
#include "unp.h"
#include "unpifiplus.h"
#include <stdio.h>
#include <string.h>


#define GET_ADDR(ifi) (ifi->ifi_addr->sin_addr.s_addr)
#define GET_MASK(ifi) (ifi->ifi_ntmaddr->sin_addr.s_addr)

int print_interfaces(struct ifi_info *ifihead) {

	struct ifi_info *ifi;
        struct sockaddr_in *sa;
        u_char *ptr;
        int i,family;
	int count = 0;
        family = AF_INET;

	printf("\n  Printing Interfaces \n");

        for (ifihead = ifi = Get_ifi_info_plus(AF_INET,1);
                ifi != NULL;ifi = ifi->ifi_next) {
                printf("%s: ",ifi->ifi_name);
                if (ifi->ifi_index != 0)
                        printf("(%d) ",ifi->ifi_index);
                printf("<");

                if (ifi->ifi_flags & IFF_UP)      printf("UP ");
                if (ifi->ifi_flags & IFF_BROADCAST) printf("BCAST ");
                if (ifi->ifi_flags & IFF_MULTICAST) printf("MCAST ");
                if (ifi->ifi_flags & IFF_LOOPBACK)  printf("LOOP ");
                if (ifi->ifi_flags & IFF_POINTOPOINT)  printf("P2P ");
                printf(">\n");

                if ((i = ifi->ifi_hlen) > 0) {
                        ptr = ifi->ifi_haddr;
                        do {
                                printf("%s%x",(i==ifi->ifi_hlen) ? " " : ":",*ptr++);
                        } while (--i > 0);
                        printf("\n");
                }
                if (ifi->ifi_mtu != 0)
                        printf("   MTU:%d \n",ifi->ifi_mtu);

                if  ((sa = ifi->ifi_addr ) != NULL)
                        printf(" IP addr:%s \n",Sock_ntop_host((struct sockaddr *) sa,sizeof(*sa)));

                if ((sa = ifi->ifi_ntmaddr) != NULL)
                        printf(" IP addr : %s\n",Sock_ntop_host((struct sockaddr *) sa,sizeof(*sa)));

                if ((sa = ifi->ifi_ntmaddr) != NULL)
                        printf(" network mask : %s\n",Sock_ntop_host((struct sockaddr *) sa,sizeof(*sa)));

                if ((sa = ifi->ifi_dstaddr) != NULL)
                        printf(" destination addr : %s \n",Sock_ntop_host((struct sockaddr *)sa,sizeof(*sa)));

		count += 1;

                }
                free_ifi_info_plus(ifihead);
		return count;

}
int is_local(struct ifi_info *ifihead,struct in_addr *iplocal,struct in_addr *ipremote) {

        unsigned long int_add,int_mask,int_subnet;
        unsigned long remote_add;
        int flag = 0;
        remote_add = htonl(ipremote->s_addr);
        unsigned long cur_subnet = GET_MASK(ifihead);
        int local_flag=0;
        // printf(" \n IP addr : %s\n",(char *)ipremote);

        struct ifi_info *newfi;
        for (newfi = ifihead;newfi != NULL;newfi = newfi->ifi_next){
	
               // printf("\n recursing ifihead\n");
                int_add = htonl(GET_ADDR(newfi));
                int_mask = htonl(GET_MASK(newfi));

                // printf("\n serv_add is %ld and client is %ld \n",remote_add,int_add);

                if (remote_add  == int_add){
                        printf(" \n Server and cleint run on same host \n");
                        inet_pton(AF_INET,"127.0.0.1", ipremote);
                        inet_pton(AF_INET,"127.0.0.1", iplocal);
                        return 1;
                }

                if (( remote_add & int_mask) == int_add & int_mask){
                        printf(" \n Server and client run on same network \n");
                        flag = 1;
                        if (cur_subnet <= int_mask) {
                                iplocal->s_addr = GET_ADDR(newfi);
                        }

                }
        }
        if (flag == 1){
                return 1;
        } else {
                iplocal->s_addr = GET_ADDR(ifihead);
                return 0;
        }


}

void addNewPacket(struct pckStruct *empty, struct pckStruct *data)
{
	empty->seqNum=data->seqNum; // 4 bytes
	empty->ackNum=data->ackNum; // 4 bytes
	empty->winSize=data->winSize; // 4 bytes
	empty->flag=data->flag; // 4 bytes
	memcpy(empty->payload,data->payload,PAYLOAD);
}
