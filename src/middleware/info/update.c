/****************************************Copyright (c)**************************************************                         
** File name:			update.c
** Descriptions:		升级的信息
**						
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-28
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include	"common.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/ctrl/gb905_control.h"
#include	"module/gb905_update/gb905_update_common.h"

#include	"middleware/info/update.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static gb905_update_param_t update_param;
static gb905_update_info_t update_info;

static unsigned char update_result;

void get_update_param(unsigned char * buf)
{
	memcpy(buf,(void *)&update_param,sizeof(gb905_update_param_t));
}

void set_update_param(unsigned char * buf)
{
	memcpy(&update_param,buf,sizeof(gb905_update_param_t));
}

void get_update_info(unsigned char * buf)
{
	memcpy(buf,(void *)&update_info,sizeof(gb905_update_info_t));
}

void set_update_info(unsigned char * buf)
{
	memcpy(&update_info,buf,sizeof(gb905_update_info_t));
}

unsigned char get_update_result(void)
{
	return update_result;
}

void set_update_result(unsigned char result)
{
	update_result = result;
}

