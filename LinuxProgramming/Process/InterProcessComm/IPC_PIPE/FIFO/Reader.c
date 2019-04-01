#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define FIFO_NAME "/tmp/my_fifo"
int main() {
    printf("\nReader:\n");
    int fd,fname;
    mkfifo(FIFO_NAME,0600);
    printf("\nStart reader\n");
    fd=open(FIFO_NAME,O_RDONLY);
    printf("\nStop reader\n");
    //int d = 0, i = 0;
    //while(read(fd,&d,sizeof(int))!=0)
    //{
        //read(fd,&d,sizeof(int));
        //printf("%d ",d);
        //usleep(10);
        //i ++;
    //}
    close(fd);
}