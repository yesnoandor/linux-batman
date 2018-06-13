/****************************************Copyright (c)**************************************************                         
** File name:			gb905_remote.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-21
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_REMOTE_H
#define _GB905_REMOTE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	unsigned char channel;
	unsigned char cmd;
}__packed gb905_remote_ctrl_body_t;


typedef struct {
	unsigned short serial_number;
	report_body_t report;
}__packed gb905_remote_ctrl_ack_body_t;

typedef  struct
{
    unsigned char start_magic_id;
    gb905_header_t header;
    gb905_remote_ctrl_ack_body_t body;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_remote_ctrl_ack_t;

unsigned char gb905_remote_control_treat(unsigned char *buf,int len);
void gb905_remote_control_ack(unsigned short seq_number);

#ifdef __cplusplus
}
#endif
	
#endif

