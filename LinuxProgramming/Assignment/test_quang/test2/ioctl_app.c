#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "ioctl_test.h"

#define DEVICE_FILENAME  "/dev/ioctldev"

int main()
{
    ioctl_test_info  info;
    int              dev;
    int              state;
    int              cnt;
    
    dev = open( DEVICE_FILENAME, O_RDWR|O_NDELAY );
    if( dev >= 0 )
    {
		printf("[USER] Write test_info with 0xAB\n");
		for ( cnt = 0; cnt < 100; cnt++ )
		{
			info.buff[cnt] = 0xAB;
		}
		info.size = 256;
		ioctl(dev, IOCTLTEST_WRITE, &info );

		sleep(1);
		printf("[USER] Read kernel's test_info \n");
		ioctl(dev, IOCTLTEST_READ, &info );
		for(cnt=0; cnt < info.size; cnt++ )
		{
			printf("[USER] %dth, info=%02X\n", cnt, (unsigned char)info.buff[cnt] );
		}
		printf("%X\n",(unsigned char)info.buff );

		close(dev);
    }

    return 0;
}

