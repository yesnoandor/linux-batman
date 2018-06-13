/****************************************Copyright (c)**************************************************                         
** File name:			gb905_inspection.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-13
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_INSPECTION_H
#define _GB905_INSPECTION_H

#ifdef __cplusplus
extern "C" {
#endif

void gb905_inspection_set_ack_info(unsigned char device_id,unsigned char* buf,unsigned char len);
void gb905_inspection_clr_ack_info(unsigned char device_id);

void gb905_inspection_device_treat(unsigned char* buf,unsigned short len);
void gb905_inspection_device_ack_treat(void);
void gb905_inspection_clr_all_ack_info(void);



#ifdef __cplusplus
}
#endif
	
#endif

