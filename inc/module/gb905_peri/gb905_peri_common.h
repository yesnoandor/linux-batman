/****************************************Copyright (c)**************************************************                         
** File name:			gb905_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-18
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_PERI_COMMON_H
#define	_GB905_PERI_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned short magic_start;
	unsigned short len;
	unsigned char type;
	unsigned char vendor;
	unsigned short cmd;
}__packed gb905_peri_header_t;


typedef struct {
	unsigned char xor;
	unsigned short magic_end;
}__packed gb905_peri_tail_t;


// 用BCD  码表示当前时间
typedef struct{
	unsigned char bcd[7];				// YYYY-MM-DD-hh-mm-ss
}__packed gb905_peri_bcd_timestamp_t;


int gb905_peri_get_a_full_msg(double_buff_mgr_t *msg);


void gb905_peri_build_header(gb905_peri_header_t* header,unsigned short cmd,unsigned short len);
void gb905_peri_build_tail(gb905_peri_tail_t * tail,unsigned char xor);
void gb905_peri_build_timestamp(gb905_peri_bcd_timestamp_t * timestamp);



#ifdef __cplusplus
}
#endif

#endif

