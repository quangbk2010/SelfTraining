#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define FIFO_NAME "/tmp/my_fifo"
int main() {
    printf("\nwriter:\n");
    int fd,fd1;
    printf("\nStart writer\n");
    fd=open(FIFO_NAME,O_WRONLY);
    //int data = 1;
    //while (1){
        //write(fd,&data,sizeof(int));
        sleep(2);
        //data++;
    //}
    //close(fd);
    printf("\nStop writer\n");
    printf("Exit\n");
}