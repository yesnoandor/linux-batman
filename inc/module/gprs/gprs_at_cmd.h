/****************************************Copyright (c)**************************************************                         
** File name:			gprs.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-06
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GPRS_AT_CMD_H
#define _GPRS_AT_CMD_H

bool gprs_get_imei(void);


int gprs_at_protocol_ayalyze(unsigned char * buf,int len);
void gprs_at_protocol_init(void);


#endif

