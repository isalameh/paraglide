#include "bmp180.h"
#include <linux_glue.h>

BMPCtl CtlReg; 

unsigned char bmp_get_id()
{
	unsigned char data;
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_DID,1,&data);
	return data;
}

unsigned char bmp_get_ver()
{
	unsigned char data;
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_VER,1,&data);
	return data;	
}

BMPCtl bmp_get_ctl_reg()
{
	BMPCtl result;
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_CTL,1,&result);
	return result;
}

void bmp_reset()
{
	unsigned char command = BMP_RESET;
	i2c_write(BMP_DEVICE_ADDR,BMP_REG_RST,1,&command);
	delay_ms(1000);
}

int bmp_init()
{
	unsigned char tmp = bmp_get_id();
	if(tmp!=BMP_DEVICE_ID)
	{
		printf("Unidentified device ID!\n");
		return -1;
	}
	printf("Valid device ID: %hu\n",tmp);
	tmp = bmp_get_ver();
	printf("Device version: %hu\n",tmp);
	CtlReg = bmp_get_ctl_reg();
	printf("OSS: %u\n",CtlReg.bit.OSS);
	return 0;
}

unsigned int bmp_read_temp()
{
	unsigned char command = BMP_CTL_TEMP;
	i2c_write(BMP_DEVICE_ADDR,BMP_REG_CTL,1,&command);	
	delay_ms(BMP_TEMP_CONVERSION_TIME);
	unsigned char data[2];
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_MSB,2,data);
	unsigned int result;
	result = ((unsigned int)(data[0])<<8)|((unsigned int)(data[1]));
	return result;
}

unsigned long bmp_read_press()
{
	unsigned long result;
	if(CtlReg.bit.OSS==0)
		{
			unsigned char command = BMP_CTL_PRE0;
			i2c_write(BMP_DEVICE_ADDR,BMP_REG_CTL,1,&command);
			delay_ms(BMP_2MS_DELAY_U8X+BMP_3MS_DELAY_U8X);
			unsigned char data[3];
			i2c_read(BMP_DEVICE_ADDR,BMP_REG_MSB,3,data);
			result = ((unsigned long)(data[0])<<16)|((unsigned long)(data[1])<<8)|((unsigned long)(data[0]));			
		}
	else
		{
		
		}
	return result;
}