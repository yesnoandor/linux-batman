/****************************************Copyright (c)**************************************************                         
** File name:			gb905_callback.c
** Descriptions:		 �绰�ز�(0x8400)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-03
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

#include	"middleware/socket/fleety_socket.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/callback/gb905_callback.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		GB905_PHONE_MAX_CHAR			20		// ���20   ���ַ�
	

// �绰�ز��·����ݽṹ
typedef struct{
	unsigned char flag;								// ��־0:��ͨͨ��;1:����
	unsigned char phone_num[GB905_PHONE_MAX_CHAR];	//�绰����(�Ϊ20byte)
}__packed gb905_callback_t;


/*
* @brief 	����͸�����ݵĴ���
* @param buf	 �绰�ز���Ϣ������ݵ�ַ
* @param len	 �绰�ز���Ϣ������ݳ���
* 
* @return  �����ն�ͨ��Ӧ���״̬
*/

unsigned char gb905_callback_treat(unsigned char *buf,int len)
{
	gb905_callback_t *callback;
	
	DbgFuncEntry();

	callback = (gb905_callback_t *)buf;

	DbgPrintf("callback flag is 0x%02x\r\n",callback->flag);
	DbgPrintf("callback phone num is %s \r\n",callback->phone_num);

	//�ж��Ƿ�Ϊ����(falg=0:��ͨͨ��;falg=1:����)
	if(callback->flag)
	{
		//�ر�������
	}
	else
	{
		//��������	
	}
	
	//����绰
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}

