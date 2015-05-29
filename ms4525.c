#include "ms4525.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MS4525_CHANNEL 2 //I2C1 map to /dev/i2c-2
#define ADD

int fd;

int ms4525_init()
{
	char portf[32];
	sprintf(portf,"/dev/i2c-%d",MS4525_CHANNEL);
	fd = open(portf,O_RDWR);
	if(fd ==-1)
	{
		perror("Open(I2C1) ");
		printf("Cannot opent port /dev/i2c-%d",MS4525_CHANNEL);
		return -1;
	}
	return 0;
}