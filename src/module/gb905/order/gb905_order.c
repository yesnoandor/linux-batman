/****************************************Copyright (c)**************************************************                         
** File name:			gb905_order.c
** Descriptions:		ƽ̨�·�����(8B00);ƽ̨�·�����(8B01);ƽ̨ȡ������(8B09);
**					��ʻԱ����(0B01);��ʻԱ��ɶ���(0B07);��ʻԱȡ������(0B08);
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-05
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
#include	"misc/endian.h"


#include	"module/gb905/gb905_common.h"
#include	"module/gb905/order/gb905_order.h"
#include	"module/gb905_ex/ui/ui_order.h"

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/info/order.h"
#include	"middleware/db/sqlite3/order_sqlite3.h"

#include	"middleware/info/toplight.h"
#include	"middleware/info/status.h"
#include	"middleware/info/notice.h"

#include	"app/order/fleety_order.h"


#define		DEBUG_Y
#include	"libs/debug.h"



//-------------------------------------------------------------------------------------------

/*
* @brief 	����������Ϣ
* @param grap_order	  ����������Ϣ�Ľṹ��ַ
* @param order_id		  �������Ķ���ID
*
*/
static void gb905_build_order_grab(gb905_grab_order_t *grap_order,unsigned int order_id)
{
	DbgFuncEntry();

    gb905_build_header(&grap_order->header,MESSAGE_ORDER_GRAB,sizeof(gb905_grab_order_t));

    grap_order->order_id = order_id;
	
	DbgFuncExit();	
}

/*
* @brief 	����ȡ����������Ϣ
* @param cancel_order	  ����ȡ��������Ϣ�Ľṹ��ַ
* @param order_id		  ��ȡ���Ķ���ID
* @param cancel_reason ȡ��������ԭ��
* 
*/
static void gb905_build_order_cancel(gb905_isu_cancel_order_t *cancel_order,unsigned int id,unsigned char reason)
{
	DbgFuncEntry();

	gb905_build_header(&cancel_order->header,MESSAGE_ORDER_CANCEL_UP,sizeof(gb905_isu_cancel_order_t));

	cancel_order->cancel_order.id = id;
	cancel_order->cancel_order.reason = reason;
	
	DbgFuncExit();	
}

/*
* @brief 	������ɶ�������Ϣ
* @param finish_order	  ������ɶ�����Ϣ�Ľṹ��ַ
* @param order_id		  ����ɵĶ���ID
* 
*/
static void gb905_build_order_finish(gb905_finish_order_t *finish_order,unsigned int order_id)
{
	DbgFuncEntry();

	gb905_build_header(&finish_order->header,MESSAGE_ORDER_FINISH,sizeof(gb905_finish_order_t));

	finish_order->order_id = order_id;
	
	DbgFuncExit();
}

/**
* @brief			���������·���Ϣ�Ĵ���
* @param buf	  	���������·���Ϣ������ݵ�ַ
* @param len	  	���������·���Ϣ������ݳ���
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_order_briefing_treat(unsigned char *buf,int len)
{
	
	DbgFuncEntry();

	set_order_brief_info(buf);
	debug_order_brief_info();

    fleety_send_briefing_judge();
   
	DbgFuncExit();

	return GB905_RESULT_OK;
}

/**
* @brief			���������·���Ϣ�Ĵ���
* @param buf	  	���������·���Ϣ������ݵ�ַ
* @param len	  	���������·���Ϣ������ݳ���
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_order_detials_treat(unsigned char *buf,int len)
{

	DbgFuncEntry();

	// ������id   ֮�����������ֶ�,���ʾδ�б�
	if(len < sizeof(gb905_order_details_t) - sizeof(char))
	{
		// ����ʧ�ܴ���
		fleety_order_grap_fail_treat();

		return GB905_RESULT_OK;
	}

    set_order_detail_info(buf);	
	debug_order_detail_info();

    fleety_send_detials_judge();
    
	DbgFuncExit();	

	return GB905_RESULT_OK;
}

/**
* @brief			����ȡ���·���Ϣ�Ĵ���
* @param buf	  	����ȡ���·���Ϣ������ݵ�ַ
* @param len	  	����ȡ���·���Ϣ������ݳ���
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_order_server_cancel_treat(unsigned char *buf,int len)
{
	unsigned int order_id;
    
	DbgFuncEntry();

	order_id = *(unsigned int *)buf;

	DbgPrintf("cancel oredr id is %d \r\n",EndianReverse32(order_id));

    fleety_order_cancel_treat();

	DbgFuncExit();

	return GB905_RESULT_OK;
}


/*
* @brief ����������Ϣ��������
* @param order_id	  ����������ID
*
*/
void gb905_order_grab_treat(void)
{
	gb905_grab_order_t grab_order;
	order_info_brief_t order_brief;

	DbgFuncEntry();

	get_order_brief_info((unsigned char *)&order_brief);

	gb905_build_order_grab(&grab_order,order_brief.id);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&grab_order,sizeof(gb905_grab_order_t));

	set_order_status_info(ORDER_STATUS_CRAB);

	DbgFuncExit();
}

/*
* @brief 					����ȡ����������Ϣ��������
* @param order_id	 		��ȡ��������ID
* @param cancel_reason	ȡ��������ԭ��
*
*/
void gb905_order_isu_cancel_treat(unsigned int id,unsigned char reason)
{

	gb905_isu_cancel_order_t cancel_order;
	
	DbgFuncEntry();

	gb905_build_order_cancel(&cancel_order,id,reason);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&cancel_order,sizeof(gb905_isu_cancel_order_t));

	set_order_status_info(ORDER_STATUS_ISU_CANCEL);
	
	DbgFuncExit();	
}

/*
* @brief ������ɶ�������Ϣ��������
* @param order_id	  ����ɶ�����ID
*
*/
void gb905_order_finish_treat(unsigned int order_id)
{
	gb905_finish_order_t finish_order;
	
	DbgFuncEntry();

	gb905_build_order_finish(&finish_order,order_id);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&finish_order,sizeof(gb905_finish_order_t));

	set_order_status_info(ORDER_STATUS_COMPLETE);
	
	DbgFuncExit();	
}


