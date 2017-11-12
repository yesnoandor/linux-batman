/****************************************Copyright (c)**************************************************                         
** File name:			device_info.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-03
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _DEVICE_INFO_H
#define	_DEVICE_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

#define		MAX_IMEI_CHARS_SIZE		16
#define		MAX_IMSI_CHARS_SIZE		16


typedef struct{
	unsigned int mtd_id;
	char imei_id[MAX_IMEI_CHARS_SIZE];
	char imsi_id[MAX_IMSI_CHARS_SIZE];
	unsigned short crc;
}__packed device_info_t,*p_device_info_t;

void init_device_info(void);
void get_device_info(device_info_t * info);
void set_device_info(device_info_t * info);


#ifdef __cplusplus
}
#endif

#endif


