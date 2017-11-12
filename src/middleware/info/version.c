/****************************************Copyright (c)**************************************************                         
** File name:			version.c
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
#include	"common.h"

#define		TEST_HW_VERSION			"v1.0.0.0"
#define		TEST_SW_VERSION			"v1.0.0.0"

void get_hw_version(char * buff)
{
	memset(buff,0x00,MAX_HW_VERSION_SIZE);
	memcpy(buff,TEST_HW_VERSION,strlen(TEST_HW_VERSION));
}

void get_sw_version(char * buff)
{
	memset(buff,0x00,MAX_SW_VERSION_SIZE);
	memcpy(buff,TEST_SW_VERSION,strlen(TEST_SW_VERSION));
}

