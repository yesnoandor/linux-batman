/****************************************Copyright (c)**************************************************                         
** File name:			gb905_params.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-14
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_PARAMS_H
#define _GB905_PARAMS_H
	
#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	unsigned short id;
	unsigned char len;
	unsigned int content;
}__packed msg_params_t;

void gb905_get_params_treat(unsigned char *buf,int len);
unsigned char gb905_set_params_treat(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif
	
#endif

