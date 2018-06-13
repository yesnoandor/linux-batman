/****************************************Copyright (c)**************************************************                         
** File name:			fleety_record.h
** Descriptions:		获取存储的信息记录并进行补传
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-17
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _FLEETY_RECORD_H
#define	_FLEETY_RECORD_H

#ifdef __cplusplus
extern "C" {
#endif
#include	"module/gb905/gb905_common.h"


#define		SUPPLEMENT_TIMEOUT		30


#define	MAX_NODE_COUNT	20

void fleety_record_init(void);
void fleety_record_deinit(void);
bool gb905_record_msg(unsigned char socket_index,gb905_header_t *header,unsigned char *buf,int len);
bool gb905_record_cancel(unsigned char socket_index,ack_body_t *ack);
int gb905_record_supplement(unsigned char socket_index);


#ifdef __cplusplus
}
#endif

#endif
