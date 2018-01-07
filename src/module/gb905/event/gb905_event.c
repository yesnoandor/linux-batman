/****************************************Copyright (c)**************************************************                         
** File name:		gb905_event.c
** Descriptions:	�¼�����(0x8301),�¼�����(0x0301) 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
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
#include	"module/gb905/event/gb905_event.h"

#include	"middleware/socket/fleety_socket.h"


#define		DEBUG_Y
#include	"libs/debug.h"

#define		GB905_EVENT_MAX_NUM					32			// ���32   ���¼�ID 
#define		GB905_EVENT_MAX_CONTENT_LEN			20			// ÿ���¼����20    ���ַ�

// �¼�������Ϣ������ݸ�ʽ
typedef struct{
    unsigned char event_id;             //�¼�������¼�ID
}__packed gb905_report_event_body_t;


// �¼������������Ϣ���ݸ�ʽ
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	gb905_report_event_body_t report_event_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_report_event_t;

// �¼�������Ϣ��ṹ
typedef struct{
    unsigned char evnet_num;            // �¼�������
    unsigned char event_list;           // �¼��б����ֽ�(�б������Ϊ499    �ֽ�)
}__packed gb905_event_t;

// �¼���������ݸ�ʽ
typedef struct{
    unsigned char content[GB905_EVENT_MAX_CONTENT_LEN];		//�¼�����(�Ϊ20�ֽ�)
}__packed gb905_event_list_t;

static int gb905_event_map = 0;
static gb905_event_list_t gb905_event_list[GB905_EVENT_MAX_NUM];

//-----
static void gb905_debug_event(void)
{
	int i;

	DbgFuncEntry();

	DbgPrintf("gb905 event map = 0x%0x\r\n",gb905_event_map);
	
	for(i=0;i<GB905_EVENT_MAX_NUM;i++)
	{
		if(gb905_event_map & (0x01 << i))
		{
			DbgPrintf("event id = %d\r\n",i);
			DbgPrintf("event content = %s\r\n",gb905_event_list[i].content);
		}
	}

	DbgFuncExit();
}

/** 
* @brief 	���������¼���������ݽṹ
* @param report_event 	�����¼��������ݽṹ��ַ
* @param event_id 		ѡ��㱨���¼�ID
*
*/
static void gb905_build_report_event(gb905_report_event_t *report_event,unsigned char evnet_id)
{
    DbgFuncEntry();

    gb905_build_header(&report_event->header,MESSAGE_EVENT_LIST_ACK,sizeof(gb905_report_event_t));

    report_event->report_event_body.event_id = evnet_id;
        
    DbgFuncExit();
}


//-----
/**
* @brief			�¼�������Ϣ�Ĵ���
* @param buf	  �¼�������Ϣ������ݵ�ַ
* @param len	  �¼�������Ϣ������ݳ���
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_set_event_treat(unsigned char *buf,int len)
{
    gb905_event_t * event;
    char *prt;
    int i,str_len;
	int index;
	
    DbgFuncEntry();

    event = (gb905_event_t *)buf;
    prt = (char *)&event->event_list;       

    // ����¼�������Ϊ0����ɾ�����������¼�
    if(0 == event->evnet_num)
    {
        gb905_event_map = 0;
		memset(gb905_event_list,0x00,sizeof(gb905_event_list));

		// ɾ���ն����ݿ��е������¼�
        
    }
    else
    {
        for(i = 0;i < event->evnet_num;i++)
    	{
    		index = *prt;

			prt++;
    		str_len = strlen(prt);
            
    		if(str_len >= GB905_EVENT_MAX_CONTENT_LEN)
    		{
    			// �������ȴ�������¼����ȵ��¼�	
    			prt += str_len+1;
    			continue;
    		}
			
			gb905_event_map |= (0x01<<index);
    		memset(gb905_event_list[index].content,0x00,sizeof(gb905_event_list[index].content));
    		strncpy((void *)gb905_event_list[index].content,prt,str_len);

			prt += str_len + 1;		// skip '\0'
    	}

		gb905_debug_event();
    }

    DbgFuncExit();
	
    return GB905_RESULT_OK;
}

/*
* @brief 	�����¼����浽������
* @param evnet_id	  ѡ��㱨���¼�ID
*
*/
void gb905_report_event_treat(unsigned char evnet_id)
{
    gb905_report_event_t report_event;
    
    DbgFuncEntry();

    gb905_build_report_event(&report_event,evnet_id);
    gb905_send_data(MAIN_SOCKET,(unsigned char *)&report_event,sizeof(gb905_report_event_t));
    
    DbgFuncExit();
}

