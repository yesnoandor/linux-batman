/****************************************Copyright (c)**************************************************                         
** File name:			endian.c
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


#define		DEBUG_Y
#include	"libs/debug.h"

/*
 * 得到当前CPU的endian类型
 * @return =1,little-endian;	=0, big-endian
 */
bool get_cpu_endian(void)
{
	union
	{
		unsigned int a;
		unsigned char b;
	}c;

	c.a = 1;

	return (c.b == 1);
}
