#include "em506.h"
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

int fd;

int em506_init(int channel)
{
	char portf[32];
	sprintf(portf, "/dev/ttyO%d", channel);
	fd = open(portf,O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		perror("Open(UART) ");
		printf("Cannot open port /dev/ttyO%d\n",channel);
		return -1;
	}
	
	struct termios options;
	// Get current settings
	if (tcgetattr(fd,&options)!=0)
	{
		close(fd);
		perror("Setup(UART) ");
		printf("Cannot get settings port /dev/ttyO%d\n",channel);
		return -1;
	}
	
	// Set baud rate
	cfsetispeed(&options, B4800);
	cfsetospeed(&options, B4800);
	
	// Settings
	options.c_cflag &= ~PARENB; //no parity
	options.c_cflag &= ~CSTOPB; //1 stop bit
	options.c_cflag &= ~CSIZE; //size 8
	options.c_cflag |= CS8;
	
	options.c_lflag &= ~ICANON;
	options.c_cc[VTIME] = 0;
	//
	
	// Apply settings NOW
	if (tcsetattr(fd, TCSANOW, &options)!=0)
	{
		close(fd);
		perror("Setup(UART)");
		printf("Cannot setup port /dev/ttyO%d\n",channel);
		return -1;
	}
	return 1;
}

void em506_read()
{
	int res;
	char buf[255];
	if ((res = read(fd,buf,255))>0)
	{
		buf[res]=0;
		printf("%s",buf,res);
		fflush(stdout);
	}
}