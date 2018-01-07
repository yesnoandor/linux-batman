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

#include	"middleware/socket/fleety_socket.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/order/gb905_order.h"

#define		DEBUG_Y
#include	"libs/debug.h"



//-------------------------------------------------------------------------------------------

/*
* @brief ��ӡ����������Ϣ����
* @param briefing	  	������Ϣ�ṹ��ַ
* @order_description	����ҵ�������ĵ�ַ
*
*/
static void gb905_debug_order_briefing(gb905_order_briefing_t *briefing,char *order_description)
{
	DbgFuncEntry();

	DbgPrintf("briefing order id is %d\r\n",EndianReverse32(briefing->id));
	DbgPrintf("briefing order type is 0x%02x\r\n",briefing->type);
	DbgPrintf("briefing order description is %s\r\n",order_description);
	
	DbgFuncExit();
}


/*
* @brief ��ӡ����������Ϣ����
* @param details	  	������Ϣ�ṹ��ַ
* @phone_num		�˿͵绰�绰�����ַ
* @order_details		ҵ��������Ϣ�����ĵ�ַ
*
*/
static void gb905_debug_order_details(gb905_order_details_t *details,char *phone_num,char *order_details)
{
	DbgFuncEntry();

	DbgPrintf("details order id is %d\r\n",EndianReverse32(details->order_id));
	DbgPrintf("details order type is 0x%02x\r\n",details->order_type);
	DbgPrintf("details order time is %02x-%02x-%02x %02x:%02x:%02x\r\n",details->ride_time.bcd[0],\
		details->ride_time.bcd[1],details->ride_time.bcd[2],details->ride_time.bcd[3],\
		details->ride_time.bcd[4],details->ride_time.bcd[5]);
	
	DbgPrintf("details order passenger lon is %d\r\n",details->passenger_lon);
	DbgPrintf("details order passenger lat is %d\r\n",details->passenger_lat);
	DbgPrintf("details order destination lon is %d\r\n",details->destination_lon);
	DbgPrintf("details order destination lat is %d\r\n",details->destination_lat);
	
	DbgPrintf("details order service fee is %d-%d\r\n",details->service_fee[0],details->service_fee[1]);

	DbgPrintf("details order phone num is %s\r\n",phone_num);
	DbgPrintf("details order details is %s\r\n",order_details);
	
	DbgFuncExit();
}


/*
* @brief 	����������Ϣ
* @param grap_order	  ����������Ϣ�Ľṹ��ַ
* @param order_id		  �������Ķ���ID
*
*/
static void gb905_build_order_grap(gb905_driver_grap_order_t *grap_order,unsigned int order_id)
{
	DbgFuncEntry();

    gb905_build_header(&grap_order->header,MESSAGE_ORDER_GRAB,sizeof(gb905_driver_grap_order_t));

    grap_order->grap_order.order_id = order_id;
	
	DbgFuncExit();	
}

/*
* @brief 	����ȡ����������Ϣ
* @param cancel_order	  ����ȡ��������Ϣ�Ľṹ��ַ
* @param order_id		  ��ȡ���Ķ���ID
* @param cancel_reason ȡ��������ԭ��
* 
*/
static void gb905_build_order_cancel(gb905_driver_cancel_order_t *cancel_order,unsigned int order_id,unsigned char cancel_reason)
{
	DbgFuncEntry();

	gb905_build_header(&cancel_order->header,MESSAGE_ORDER_CANCEL_UP,sizeof(gb905_driver_cancel_order_t));

	cancel_order->cancel_order.order_id = order_id;
	cancel_order->cancel_order.cancel_reason = cancel_reason;
	
	DbgFuncExit();	
}

/*
* @brief 	������ɶ�������Ϣ
* @param finish_order	  ������ɶ�����Ϣ�Ľṹ��ַ
* @param order_id		  ����ɵĶ���ID
* 
*/
static void gb905_build_order_finish(gb905_driver_finish_order_t *finish_order,unsigned int order_id)
{
	DbgFuncEntry();

	gb905_build_header(&finish_order->header,MESSAGE_ORDER_FINISH,sizeof(gb905_driver_finish_order_t));

	finish_order->finish_order.order_id = order_id;
	
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
	gb905_order_briefing_t *briefing;
	char *point;
	
	DbgFuncEntry();

	briefing = (gb905_order_briefing_t *)buf;
	point = (char *)&briefing->description;	//ָ��ָ��ҵ������������

	gb905_debug_order_briefing(briefing,point);
	
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
	gb905_order_details_t *details;
	char *phone_point;
	char *details_point;
	
	DbgFuncEntry();

	//������id֮�����������ֶ�,���ʾδ�б�
	if(len == sizeof(details->order_id))
	{
	
		//����ʧ�ܴ���

		return GB905_RESULT_OK;
	}

	details = (gb905_order_details_t *)buf;

	phone_point = (char *)&details->details;//ָ��ָ��绰����

	details_point = phone_point + strlen(phone_point) +1;//ָ��ָ��ҵ���������� 1:skip '/0'

	gb905_debug_order_details(details,phone_point,details_point);
	
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
unsigned char gb905_order_cancel_treat(unsigned char *buf,int len)
{
	gb905_order_cancel_t *cancel;
	DbgFuncEntry();

	cancel = (gb905_order_cancel_t *)buf;

	DbgPrintf("cancel oredr id is %d \r\n",EndianReverse32(cancel->order_id));

	//������ʾ��ʻԱ�����ѱ�ȡ��
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}


/*
* @brief ����������Ϣ��������
* @param order_id	  ����������ID
*
*/
void gb905_driver_grap_order_treat(unsigned int order_id)
{
	gb905_driver_grap_order_t grap_order;

	DbgFuncEntry();

	gb905_build_order_grap(&grap_order,order_id);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&grap_order,sizeof(gb905_driver_grap_order_t));
	
	DbgFuncExit();
}

/*
* @brief 					����ȡ����������Ϣ��������
* @param order_id	 		��ȡ��������ID
* @param cancel_reason	ȡ��������ԭ��
*
*/
void gb905_driver_cancel_order_treat(unsigned int order_id,unsigned char cancel_reason)
{

	gb905_driver_cancel_order_t cancel_order;
	
	DbgFuncEntry();

	gb905_build_order_cancel(&cancel_order,order_id,cancel_reason);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&cancel_order,sizeof(gb905_driver_cancel_order_t));
	
	DbgFuncExit();	
}

/*
* @brief ������ɶ�������Ϣ��������
* @param order_id	  ����ɶ�����ID
*
*/
void gb905_driver_finish_order_treat(unsigned int order_id)
{

	gb905_driver_finish_order_t finish_order;
	
	DbgFuncEntry();

	gb905_build_order_finish(&finish_order,order_id);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&finish_order,sizeof(gb905_driver_finish_order_t));
	
	DbgFuncExit();	
}


