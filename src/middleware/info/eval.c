/****************************************Copyright (c)**************************************************                         
** File name:			eval.c
** Descriptions:		获得当前运行的评价信息
**						
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-16
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

/**
* 服务评价
*	= 0x00, 没有做出评价 
*	= 0x01, 满意
*	= 0x02, 一般
*	= 0x03, 不满意
*	= 0x04, 投诉
*/
static unsigned char eval_state;

unsigned char get_eval_state(void)
{
	return eval_state;
}

void set_eval_state(unsigned char state)
{
	eval_state = state;
}

