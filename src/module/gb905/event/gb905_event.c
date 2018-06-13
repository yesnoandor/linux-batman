/****************************************Copyright (c)**************************************************                         
** File name:		gb905_event.c
** Descriptions:	事件设置(0x8301),事件报告(0x0301) 
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
#include	"module/gb905_ex/ui/ui_event.h"

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/db/sqlite3/event_sqlite3.h"

#include	"middleware/info/event.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		GB905_EVENT_MAX_NUM					32			// 最多32   个事件ID 
#define		GB905_EVENT_MAX_CONTENT_LEN			20			// 每个事件最多20    个字符

// 事件报告消息体的数据格式
typedef struct{
    unsigned char event_id;             //事件报告的事件ID
}__packed gb905_report_event_body_t;


// 事件报告的整个消息数据格式
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	gb905_report_event_body_t report_event_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_report_event_t;

// 事件设置消息体结构
typedef struct{
    unsigned char evnet_num;            // 事件总数量
    unsigned char event_list;           // 事件列表首字节(列表长度最大为499    字节)
}__packed gb905_event_t;


/** 
* @brief 	构造整个事件报告的数据结构
* @param report_event 	整个事件报告数据结构地址
* @param event_id 		选择汇报的事件ID
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
* @brief			事件设置消息的处理
* @param buf	  事件设置消息体的数据地址
* @param len	  事件设置消息体的数据长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_set_event_treat(unsigned char *buf,int len)
{
    gb905_event_t * event;
    save_event_t gb905_event_list;
    char *prt;
    int i,str_len;
	
    DbgFuncEntry();

    event = (gb905_event_t *)buf;
    prt = (char *)&event->event_list;       

	memset(&gb905_event_list,0x00,sizeof(save_event_t));
    
    // 如果事件总数量为0，则删除现有所有事件
    if(0 == event->evnet_num)
    {
		// 删除终端数据库中的所有事件
        event_sqlite3_delete_record();
    }
    else
    {	
        for(i = 0;i < event->evnet_num;i++)
    	{
    		gb905_event_list.event_msg[i].id = *prt;

			prt++;
    		str_len = strlen(prt);
            
    		if(str_len >= GB905_EVENT_MAX_CONTENT_LEN)
    		{
    			// 跳过长度大于最大事件长度的事件	
    			prt += str_len+1;
                event->evnet_num --;
    			continue;
    		}
			
    		memset(gb905_event_list.event_msg[i].content,0x00,sizeof(gb905_event_list.event_msg[i].content));
    		strncpy((void *)gb905_event_list.event_msg[i].content,prt,str_len);

			//判断该事件ID是否已经存在
			if(!event_sqlite3_judge_event_exist((int)gb905_event_list.event_msg[i].id))
			{
				//如果不存在,则直接插入该新事件
				event_sqlite3_insert_record((int)gb905_event_list.event_msg[i].id,prt,str_len);
			}
			else
			{
				//如果存在,则更新该事件的内容
				event_sqlite3_update_by_id((int)gb905_event_list.event_msg[i].id,prt);
			}
			prt += str_len + 1;		// skip '\0'
    	}

        gb905_event_list.num = event->evnet_num;//符合要求的事件数量(内容不大于20字节)

        event_set_info(&gb905_event_list);

        //发送到UI显示当前下发的事件列表
    	ui_send_current_event_list();

        //gb905_debug_event();
		//event_sqlite3_restore_record();//for test
    }

    DbgFuncExit();
	
    return GB905_RESULT_OK;
}

/*
* @brief 	发送事件报告到服务器
* @param evnet_id	  选择汇报的事件ID
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


