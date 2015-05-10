#include "ubloxlea6.h"
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

int uart_fd;

void ubloxlea6_checksum(int *c_a,int *c_b, int length, unsigned char const *buffer)
{
	*c_a = 0;
	*c_b = 0;
	int i;
	for (i=2;i<length;i++)
	{
		*c_a = *c_a + buffer[i];
		*c_b = *c_b + *c_a;
	}
}

int ubloxlea6_init(int channel)
{
	char portf[32];
	sprintf(portf, "/dev/ttyS%d", channel);
	int uart_fd = open(portf,O_RDWR | O_NOCTTY | O_NDELAY);
	if (uart_fd == -1)
	{
		perror("open(UART)");
		printf("Cannot open port /dev/ttyS&d\n",channel);
		return -1;
	}
	
	ubloxlea6_cfg_ant_poll();
	char buffer[32];
	int n = read(uart_fd,buffer,sizeof(buffer));
	if(n<0)
	{
		printf("Read UART channel %d failed\n",channel);
		return -1;
	}
	else
		printf(buffer);
	printf("\n");
	return 0;
}


int ubloxlea6_cfg_ant_poll()
{
	char buffer[6];
	buffer[0]=HEAD_a;
	buffer[1]=HEAD_b;
	buffer[2]=ID_CFG_ANT_a;
	buffer[3]=ID_CFG_ANT_b;
	ubloxlea6_checksum(buffer+4,buffer+5,4,buffer);
	printf(buffer);
	printf("\n");
	int n = write(uart_fd,buffer,sizeof(buffer));
	return n;
}