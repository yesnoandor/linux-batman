/****************************************Copyright (c)**************************************************                         
** File name:			gb905_notice.c
** Descriptions:		����905    �е��ı���Ϣ�·�
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2017-12-29
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
#include	"module/gb905/notice/gb905_notice.h"

#define		DEBUG_Y
#include	"libs/debug.h"

// �ı���Ϣ�·���־
typedef union 
{
	unsigned char whole;
	
	struct
	{
		// low --> high
		unsigned char urgent:1;		        // bit0 : ����
		unsigned char reserved:1;			// bit1 : Ԥ��
		unsigned char lcd:1;                // bit2 : ��ʾװ����ʾ
		unsigned char tts:1;                // bit3 : �����ϳɲ���
		unsigned char adv:1;                // bit4 : �������ʾ
		unsigned char :0;                   // bit5~bit7:Ԥ��
    }flag;
}gb905_notice_flag_t;


// �ı���Ϣ�·����ݽṹ
typedef struct{
	gb905_notice_flag_t flag;				// ��־
	int content;							// �ı���Ϣ(�Ϊ499byte)
}__packed gb905_notice_t;


/*
* @brief 	ƽ̨�·��ı���Ϣ�Ĵ���
* @param buf	  �·��ı���Ϣ��Ĳ�����ַ
* @param len	  �·��ı���Ϣ��Ĳ�������
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_notice_treat(unsigned char *buf,int len)
{
    gb905_notice_t *notice;

    DbgFuncEntry();

    notice = (gb905_notice_t *)buf;

    DbgPrintf("flag = 0x%0x\r\n",notice->flag.whole);
    DbgPrintf("content = %s\r\n",(unsigned char *)&notice->content);

    DbgFuncExit();
	
    return GB905_RESULT_OK;
}

