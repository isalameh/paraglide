#ifndef ubloxlea6_H
#define ubloxlea6_H


#define HEAD_a 0xB5
#define HEAD_b 0x62
#define ID_CFG_ANT_a 0x06
#define ID_CFG_ANT_b 0x13 
int ubloxlea6_init(int channel);


//void ubloxlea6_checksum(int *c_a,int *c_b, int length, unsigned char const *buffer);

int ubloxlea6_cfg_ant_poll();

#endif /*ubloxlea6_H*/