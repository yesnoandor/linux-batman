/****************************************Copyright (c)**************************************************                         
** File name:			fleety_order.c
** Descriptions:		��������
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-02-07
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
#include	"module/gb905/order/gb905_order.h"

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_history_info.h"
#include	"module/gb905_ex/ui/ui_order.h"
#include	"module/gb905_ex/ui/ui_notice.h"

#include	"middleware/db/sqlite3/order_sqlite3.h"
#include	"middleware/info/toplight.h"
#include	"middleware/info/order.h"
#include	"middleware/info/status.h"


#include	"app/order/fleety_order.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define ORDER_GRAP_REMIND_INFO	    "��������ʧ�ܣ�����"
#define ORDER_CANCEL_REMIND_INFO	"������ȡ��������"
#define ORDER_CANCEL_FINISH_INFO	"��������ɣ�����"


//�ж��Ƿ���Ҫ��������Ϣ���͵�UI
void fleety_send_briefing_judge(void)
{
 	order_info_brief_t order_brief;

	DbgFuncEntry();

	get_order_brief_info((unsigned char *)&order_brief);

    //�ڵ�¼�����ǿճ���״̬��
	if(get_login_mode()&& !get_loading_status())
	{
        //�ڵ�����
        if(get_order_state())
        {
            //������ֻ�ܽ���ԤԼ����
            if(order_brief.type==ORDER_TYPE_RESERVED)
            {
                //���ö���״̬
                set_order_status_info(ORDER_STATUS_BRIEF);
                
                //���ͼ�����Ϣ��UI
                ui_send_order_briefing();
            }
        }
        else
        {
            if(order_brief.type==ORDER_TYPE_APPOINT)
            {
                //���ͼ�����Ϣ��UI
                ui_send_order_briefing();

                //�Զ�����
                gb905_order_grab_treat(); 
            }
            else
            {
                //���ö���״̬
                set_order_status_info(ORDER_STATUS_BRIEF);
                
                //���ͼ�����Ϣ��UI
                ui_send_order_briefing();   
            }
          
        }
        
    }

    DbgFuncExit();
   
}

//�ж��Ƿ���Ҫ��������Ϣ���͵�UI
void fleety_send_detials_judge(void)
{
    order_info_detail_t detail_info;
    DbgFuncEntry();

    get_order_detail_info((unsigned char *)&detail_info);
    
    //����״̬Ϊ�����ɹ�
    if(get_order_status_info()==ORDER_STATUS_CRAB_ACK)
    {

        //����������Ϣ��UI
    	ui_send_order_detials();

        //�յ�����󽫶�����Ϣ�������ݿ�
    	order_sqlite3_insert_record();

        if(detail_info.order_type==ORDER_TYPE_REAL_TIME||detail_info.order_type==ORDER_TYPE_APPOINT)
        {
            //���ö���״̬
        	set_toplight_operation_status(TOPLIGHT_STATUS_ORDER);

            //���õ���״̬λ
            set_order_state(true);

            set_order_status_info(ORDER_STATUS_DETAIL);
        }

    }

    DbgFuncExit();
}

//��������ʧ�ܴ���
void fleety_order_grap_fail_treat(void)
{
    DbgFuncEntry();

	set_order_status_info(ORDER_STATUS_IDLE);

    //��������ʧ����ʾ��UI
    ui_send_terminal_remind((unsigned char*)ORDER_GRAP_REMIND_INFO,sizeof(ORDER_GRAP_REMIND_INFO));
        
    DbgFuncExit();
    
}

//����ȡ������(ƽ̨�·�ȡ��)
void fleety_order_cancel_treat(void)
{
	DbgFuncEntry();

    set_order_status_info(ORDER_STATUS_SERVER_CANCEL);

    //ƽ̨��ȡ���ö������������ݿ�״̬
    order_sqlite3_update_state();

	//������ʾ��ʻԱ�����ѱ�ȡ��
    ui_send_terminal_remind((unsigned char*)ORDER_CANCEL_REMIND_INFO,sizeof(ORDER_CANCEL_REMIND_INFO));

    //���õ���״̬λ
    set_order_state(false);

    //���ƻָ��ճ�״̬
    set_toplight_operation_status(TOPLIGHT_STATUS_EMPTY);

    DbgFuncExit();

}

//������ɴ���
void fleety_order_finish_treat(void)
{
    DbgFuncEntry();

	set_order_status_info(ORDER_STATUS_COMPLETE_ACK);
    
    //�յ���ɶ�����Ӧ�𣬸������ݿ�״̬
    order_sqlite3_update_state();

	//������ʾ��ʻԱ���������
    ui_send_terminal_remind((unsigned char*)ORDER_CANCEL_FINISH_INFO,sizeof(ORDER_CANCEL_FINISH_INFO));

    //���õ���״̬λ
    set_order_state(false);
    
    DbgFuncExit();
}

void fleety_order_start_treat(unsigned int order_id)
{
    
    DbgFuncEntry();

    //���ö���״̬
	set_toplight_operation_status(TOPLIGHT_STATUS_ORDER);

    //���õ���״̬λ
    set_order_state(true);

    set_order_status_info(ORDER_STATUS_DETAIL);
    
    DbgFuncExit();
    
}
