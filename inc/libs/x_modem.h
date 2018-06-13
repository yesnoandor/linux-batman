#ifndef _X_MODEM_H
#define	_X_MODEM_H

#ifdef __cplusplus
extern "C" {
#endif

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1
#define MAXRETRANS 25

int xmodemReceive(unsigned char *dest, int destsz);
int xmodemTransmit(unsigned char *src, int srcsz);

int xmodem_read(unsigned char *buf,int byte);
int xmodem_write(unsigned char *buf,int byte);

unsigned short crc16_ccitt(const char *buf, int len);

#ifdef __cplusplus
}
#endif

#endif

