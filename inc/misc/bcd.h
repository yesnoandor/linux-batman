/****************************************Copyright (c)**************************************************                         
** File name:			bcd.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _BCD_H
#define	_BCD_H


#ifdef __cplusplus
extern "C" {
#endif

int decimal2bcd(unsigned int dec,unsigned char *buf, int len);
unsigned int bcd2decimal(unsigned char *buf, int len);
bool is_bcd(unsigned char *buf, int len);
void bcdtime_to_char(unsigned char *bcd_time,unsigned char* str_time);


#ifdef __cplusplus
}
#endif



#endif

