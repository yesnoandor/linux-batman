/****************************************Copyright (c)**************************************************                         
** File name:			eval.c
** Descriptions:		��õ�ǰ���е�������Ϣ
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
* ��������
*	= 0x00, û���������� 
*	= 0x01, ����
*	= 0x02, һ��
*	= 0x03, ������
*	= 0x04, Ͷ��
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

