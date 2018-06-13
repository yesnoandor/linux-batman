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

#include	"middleware/db/sqlite3/notice_sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/notice/gb905_notice.h"
#include	"module/gb905_ex/ui/ui_notice.h"

#include	"middleware/info/notice.h"

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
    int data_len;
    DbgFuncEntry();


    notice = (gb905_notice_t *)buf;
    data_len = strlen((char *)&notice->content);
    DbgPrintf("flag = 0x%0x len %d\r\n",notice->flag.whole,data_len);

    //����ı���Ϣ����Ϊ449byte
    if(data_len>NOTICE_MAX_LEN -1)// skip '\0'
    {
        return GB905_RESULT_FAIL;
    }
    
    //DbgPrintf("content = %s\r\n",(unsigned char *)&notice->content);
	
	if(notice->flag.flag.urgent)
	{
		//���͵�ǰ�ı���Ϣ��UI(��������:�ı�+������ʾ)
		ui_send_current_notice_list(buf,len,DISPLAY_TYPE_TTS_NOTICE);
	}
	else
	{
		if((notice->flag.flag.lcd||notice->flag.flag.adv)&&notice->flag.flag.tts)
		{
			//���͵�ǰ�ı���Ϣ��UI(�ı�+������ʾ)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_TTS_NOTICE);			
		}
		else if((notice->flag.flag.lcd||notice->flag.flag.adv))
		{
			//���͵�ǰ�ı���Ϣ��UI(���ı���ʾ)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_NOTICE);
		}
		else if(notice->flag.flag.tts)
		{
			//���͵�ǰ�ı���Ϣ��UI(��������ʾ)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_TTS);			
		}
		else
		{
			//���͵�ǰ�ı���Ϣ��UI(�������:���ı���ʾ)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_NOTICE);
		}
	}
	
	//�����ı���Ϣ�����ݿ�
	notice_sqlite3_insert_record((int)notice->flag.whole,(unsigned char *)&notice->content,strlen((char *)&notice->content));

	//notice_sqlite3_restore_record();//for test
	
    DbgFuncExit();
	
    return GB905_RESULT_OK;
}

